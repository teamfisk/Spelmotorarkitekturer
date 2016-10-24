#include "../STUFF/STUFF.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

int main()
{
    //using namespace boost::lambda;

    
    STUFF::Header header;

    std::vector<STUFF::Entry> entries;


    boost::filesystem::path dir("Resources");


    std::cout << dir << std::endl;

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

    std::cout << "Number of files: " << entries.size() << std::endl;
    header.NumEntries = entries.size();

    for (auto it : entries) {
        std::cout << std::string(it.FilePath, it.PathLength )<< "\tSize: " << it.Size << "\tOffset: " << it.Offset <<std::endl;
    }

    //Calculate size of bundle file
    std::uint64_t bundleSize = 0;
    bundleSize += sizeof(header.Signature);
    bundleSize += sizeof(header.Version);
    bundleSize += sizeof(header.NumEntries);

    for (auto entry : entries) {
        bundleSize += sizeof(entry.PathLength);
        bundleSize += strlen(entry.FilePath);
        bundleSize += sizeof(entry.Offset);
        bundleSize += sizeof(entry.Size);
        bundleSize += entry.Size; // size of data
    }

    //Allocate bundle
    

    std::ofstream ofile("bundle.STUFF", std::ios::binary);

    //Write header
    ofile << header.Signature;
    ofile << header.Version;
    ofile << header.NumEntries;

    //Write file entries
    for (auto entry : entries) {
        ofile << entry.PathLength;
        ofile << entry.FilePath;
        ofile << entry.Offset;
        ofile << entry.Size;
    }

    //Write data
    for (auto entry : entries) {

        std::ifstream src(std::string(entry.FilePath, entry.PathLength), std::ios::binary);
        ofile << src.rdbuf();
    }
    ofile.close();

    return 0;
}
