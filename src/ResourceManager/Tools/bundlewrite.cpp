#include "../STUFF/STUFF.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/algorithm/string/case_conv.hpp>

int main()
{
    //using namespace boost::lambda;

    std::size_t offset = 0;
    STUFF::Header header; offset += sizeof(header);

    std::vector<STUFF::Entry> entries;


    boost::filesystem::path dir("../Resources");


    std::cout << dir << std::endl;

    for (boost::filesystem::directory_iterator it(dir);
        it != boost::filesystem::directory_iterator(); ++it) {
        if (boost::filesystem::is_regular_file(it->status())) {
            STUFF::Entry entry;
            entry.FilePath = it->path().filename().string;
            //files.push_back(it->path().filename());
        }
    }

    std::cout << "Number of files: " << files.size() << std::endl;
    header.NumEntries = files.size();

    for (auto it : files) {
        std::cout << it << std::endl;
    }


    // std::cout << "Antalet filer i mappen: " << header.NumEntries;


    FILE* file = fopen("bundle.stuff", "w");



    STUFF::Entry entry; offset += sizeof(entry) - sizeof(entry.FilePath);
    entry.FilePath = "things/thing.txt"; offset += strlen(entry.FilePath);
    entry.PathLength = strlen(entry.FilePath);
    entry.Offset = offset;
    //entry.Size = strlen(thing) + 1;

    //Write header
    fwrite(&header.Signature, sizeof(header.Signature), 1, file);
    fwrite(&header.Version, sizeof(header.Version), 1, file);
    fwrite(&header.NumEntries, sizeof(header.NumEntries), 1, file);

    //Write file entries
    fwrite(&entry.PathLength, sizeof(entry.PathLength), 1, file);
    fwrite(entry.FilePath, strlen(entry.FilePath), 1, file);
    fwrite(&entry.Offset, sizeof(entry.Offset), 1, file);
    fwrite(&entry.Size, sizeof(entry.Size), 1, file);


    //Write data
    //fwrite(thing, strlen(thing) + 1, 1, file);

    fclose(file);

    std::cout << "Offset: " << entry.Offset << std::endl;
    std::cout << "Size: " << entry.Size << std::endl;



    return 0;
}
