#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "../STUFF/STUFF.h"

int main(int argc, char* argv[])
{
    bool verbose = false;

    for(int i = 1; i < argc; i++) {
        if(argv[i] == "-v" || argv[i] == "-V" || argv[i] == "--verbose") {
            verbose = true;
        }
    }

    STUFF::Header header;
    std::vector<STUFF::Entry> entries;
    boost::filesystem::path dir("Resources");

    if (verbose) {
        std::cout << "Reading files from directory: ";
        std::cout << dir << std::endl;
    }

    std::uint64_t fileOffset = 0;

    for (boost::filesystem::recursive_directory_iterator  it(dir);
        it != boost::filesystem::recursive_directory_iterator(); ++it) {
        if (boost::filesystem::is_regular_file(it->status())) {
            std::string pathStr = it->path().string();
            boost::replace_all(pathStr, "\\", "/");

            STUFF::Entry entry;
            entry.PathLength = pathStr.length();
            entry.FilePath = new char[entry.PathLength];
            memcpy(entry.FilePath, pathStr.c_str(), entry.PathLength);

            entry.Size = boost::filesystem::file_size(it->path());

            entry.Offset = fileOffset;
            fileOffset += entry.Size;

            entries.push_back(entry);
        }
    }

    header.NumEntries = entries.size();

    if (verbose) {
        std::cout << "Number of files: " << entries.size() << std::endl;

        for (auto it : entries) {
            std::cout << std::string(it.FilePath, it.PathLength)<< "\tSize: " << it.Size << "\tOffset: " << it.Offset <<std::endl;
        }
    }

    std::ofstream ofile("TestBundle.stuff", std::ios::binary);

    //Write header

    ofile.write(header.Signature, sizeof(header.Signature));
    ofile.write(reinterpret_cast<char*>(&header.Version), sizeof(header.Version));
    ofile.write(reinterpret_cast<char*>(&header.NumEntries), sizeof(header.NumEntries));


    //Write file entries
    for (auto entry : entries) {
        ofile.write(reinterpret_cast<char*>(&entry.PathLength), sizeof(entry.PathLength));
        ofile.write(reinterpret_cast<char*>(entry.FilePath), entry.PathLength);
        ofile.write(reinterpret_cast<char*>(&entry.Offset), sizeof(entry.Offset));
        ofile.write(reinterpret_cast<char*>(&entry.Size), sizeof(entry.Size));
    }

    //Write data
    for (auto entry : entries) {
        std::ifstream src(std::string(entry.FilePath, entry.PathLength), std::ios::binary);

        char* buffer = new char[entry.Size];
        src.read(buffer, entry.Size);
        ofile.write(buffer, entry.Size);
        delete[] buffer;
        src.close();
    }
    ofile.close();

    return 0;
}
