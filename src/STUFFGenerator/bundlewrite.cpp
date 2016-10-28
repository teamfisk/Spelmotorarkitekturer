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

void addFileEntry(boost::filesystem::path p, std::vector<STUFF::Entry> &entries, std::uint64_t &offset);

int main(int argc, char* argv[])
{
    bool verbose = false;
    std::string outputFile = "Bundle.stuff";
    std::string inputDirectory = "Resources";
    std::vector<boost::filesystem::path> inputFiles;


    try {
        desc.add_options()
            ("verbose,v", "Print more")
            ("output-file,o", boost::program_options::value<std::string>(), "output file")
            ;

        boost::program_options::variables_map vm;

        try {
            boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

            if (vm.count("verbose")) {
                verbose = true;
            }

            if (vm.count("output-file")) {
                outputFile = vm["output-file"].as<std::string>();
            }

            if (vm.count("input-directory")) {
                inputDirectory = vm["input-directory"].as<std::string>();

            }

            for (int i = 1 + vm.size(); i < argc; i++) {
                inputFiles.push_back(boost::filesystem::relative(boost::filesystem::path(argv[i])));
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

    STUFF::Header header;
    std::vector<STUFF::Entry> entries;
    std::uint64_t fileOffset = 0;

    std::string singleFolder = "";

    for (int i = 0; i < inputFiles.size(); i++) {
        boost::filesystem::path currentPath = inputFiles.at(i);

        if (!boost::filesystem::exists(currentPath)) {
            continue;
        }

        if (!boost::filesystem::is_directory(currentPath)) {
            if (boost::filesystem::is_regular_file(currentPath)) { // Is a file
                addFileEntry(currentPath, entries, fileOffset);
            }
        } else { // Is a directory
            for (boost::filesystem::recursive_directory_iterator  it(currentPath);
                it != boost::filesystem::recursive_directory_iterator(); ++it) {
                boost::filesystem::path p = (*it);
                if (boost::filesystem::is_regular_file(p)) {
                    if (inputFiles.size() == 1) {
                        outputFile = currentPath.filename().string() + ".stuff";
                        singleFolder = currentPath.filename().string() + "/";
                    }
                    addFileEntry(p, entries, fileOffset);
                }
            }
        }
    }

    if (verbose) {
        std::cout << "Number of files: " << entries.size() << std::endl;
    }


    header.NumEntries = entries.size();
    std::ofstream ofile(outputFile, std::ios::binary);

    //Write header
    ofile.write(header.Signature, sizeof(header.Signature));
    ofile.write(reinterpret_cast<char*>(&header.Version), sizeof(header.Version));
    ofile.write(reinterpret_cast<char*>(&header.NumEntries), sizeof(header.NumEntries));


    //Write file entries
    for (auto entry : entries) {

        if (singleFolder != "") {
            std::string filepath = std::string(entry.FilePath, entry.PathLength);
            boost::algorithm::replace_all(filepath, singleFolder, "");
            std::uint16_t pathlength = filepath.size();

            char* filepathC = new char[pathlength];
            memcpy(filepathC, filepath.c_str(), pathlength);

            if (verbose) {
                std::cout << std::string(filepathC, pathlength)<< "\tSize: " << entry.Size << "\tOffset: " << entry.Offset <<std::endl;
            }

            ofile.write(reinterpret_cast<char*>(&pathlength), sizeof(pathlength));
            ofile.write(reinterpret_cast<char*>(filepathC), pathlength);

        } else {
            if (verbose) {
                std::cout << std::string(entry.FilePath, entry.PathLength)<< "\tSize: " << entry.Size << "\tOffset: " << entry.Offset <<std::endl;
            }

            ofile.write(reinterpret_cast<char*>(&entry.PathLength), sizeof(entry.PathLength));
            ofile.write(reinterpret_cast<char*>(entry.FilePath), entry.PathLength);
        }
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

    if (verbose) { 
        std::cin.get();
    }
    
    return 0;
}

void addFileEntry(boost::filesystem::path p, std::vector<STUFF::Entry> &entries, std::uint64_t &offset)
{
    std::string pathStr = p.string();
    boost::replace_all(pathStr, "\\", "/");
    STUFF::Entry entry;
    entry.PathLength = pathStr.length();
    entry.FilePath = new char[entry.PathLength];
    memcpy(entry.FilePath, pathStr.c_str(), entry.PathLength);
    entry.Size = boost::filesystem::file_size(p);
    entry.Offset = offset;
    offset += entry.Size;
    entries.push_back(entry);
}
