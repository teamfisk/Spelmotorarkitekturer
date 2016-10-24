#include "../STUFF/STUFF.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/algorithm/string/replace.hpp>

int main()
{
    //using namespace boost::lambda;

    
    STUFF::Header header;

    std::vector<STUFF::Entry> entries;


    boost::filesystem::path dir("Resources");


    std::cout << dir << std::endl;

    std::uint64_t fileOffset = 0;

    for (boost::filesystem::directory_iterator it(dir);
        it != boost::filesystem::directory_iterator(); ++it) {
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


    //Allocate bundle


    FILE* file = fopen("bundle.stuff", "w");


    //Write header
    fwrite(&header.Signature, sizeof(header.Signature), 1, file);
    fwrite(&header.Version, sizeof(header.Version), 1, file);
    fwrite(&header.NumEntries, sizeof(header.NumEntries), 1, file);

    //Write file entries
    for (auto entry : entries) {
        fwrite(&entry.PathLength, sizeof(entry.PathLength), 1, file);
        fwrite(entry.FilePath, strlen(entry.FilePath), 1, file);
        fwrite(&entry.Offset, sizeof(entry.Offset), 1, file);
        fwrite(&entry.Size, sizeof(entry.Size), 1, file);
    }

    //Write data
    for (auto entry : entries) {



        //fwrite(thing, strlen(thing) + 1, 1, file);
    }
    fclose(file);




    return 0;
}
