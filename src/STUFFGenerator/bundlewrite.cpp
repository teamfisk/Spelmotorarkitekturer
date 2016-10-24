#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/program_options.hpp>

#include "ResourceManager/STUFF/STUFF.h"

const size_t ERROR_IN_COMMAND_LINE = 1;
const size_t SUCCESS = 0;
const size_t ERROR_UNHANDLED_EXCEPTION = 2;

// Declare the supported options.
boost::program_options::options_description desc("Allowed options");



int main(int argc, char* argv[])
{
    bool verbose = false;
    std::string outputFile = "Bundle.stuff";
    std::string inputDirectory = "Resources";

    try {
        desc.add_options()
            ("verbose,v", "Print more")
            ("input-directory,i", boost::program_options::value<std::string>(), "input directory")
            ("output-file,o", boost::program_options::value<std::string>(), "output file")
            ;

        boost::program_options::variables_map vm;

        try {
            boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

            if (vm.count("verbose")) {
                verbose = true;
            }

            if(vm.count("output-file")) {
                outputFile = vm["output-file"].as<std::string>();
            }

            if (vm.count("input-directory")) {
                inputDirectory = vm["input-directory"].as<std::string>();
            }

            boost::program_options::notify(vm);
        } catch (boost::program_options::error& e) {
            std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
            std::cerr << desc << std::endl;
            return ERROR_IN_COMMAND_LINE;
        }


    } catch (std::exception& e) {
        std::cerr << "Unhandled Exception reached the top of main: "
            << e.what() << ", application will now exit" << std::endl;
        return ERROR_UNHANDLED_EXCEPTION;

    }
    
    

    for(int i = 1; i < argc; i++) {
        if(argv[i] == "-v" || argv[i] == "-V" || argv[i] == "--verbose") {
            verbose = true;
        }
    }

    STUFF::Header header;
    std::vector<STUFF::Entry> entries;
    boost::filesystem::path dir(inputDirectory);

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

    std::ofstream ofile(outputFile, std::ios::binary);

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
        ofile << src.rdbuf();
        src.close();
    }
    ofile.close();


    return 0;
}
