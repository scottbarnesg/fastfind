#include <iostream>
#include <filesystem>
#include <thread>

#include "parser.hpp"
#include "index.hpp"

void parse_files() {
    // Parse files on disk
    Parser parser = Parser();
    // std::string searchDir = std::filesystem::current_path();
    std::string searchDir = "/home/scottgbarnes/";
    std::cout << "Parsing files in " << searchDir << " ..." << std::flush;;
    parser.findFiles(searchDir); 
    std::cout << " done. Found " << parser.getFilePaths().size() << " files." << std::endl;
    // Generate search index
    std::cout << "Generating search index..." << std::flush;
    Index index = Index();
    index.addFiles(parser.getFilePaths());
    std::cout << " done." << std::endl;
    // Write files to disk
    index.saveToDisk();
}

int main(int argc, char** argv) {
    Index index = Index();
    std::thread parseFilesThread;
    if (index.canLoadFromFile()) {
        // Load data from file
        std::cout << "Loading search index from disk...";
        index.loadFromDisk();
        std::cout << " done." << std::endl;
    }
    else {
        // Parse file system in main thread for first time setup
        std::cout << "No existing search index found, running first time build. This may take a minute..." << std::endl;
        parse_files();
        // Load data from file
        std::cout << "Loading search index from disk...";
        index.loadFromDisk();
        std::cout << " done." << std::endl;
    }
    
    /*
    while(true) {
        std::string input;
        std::cout << "Enter a search term: ";
        std::cin >> input;
        std::vector<std::string> results = index.getFilenamesBySearchTerm(input);
        std::cout << "Results: " << std::endl;
        for (std::string result : results) {
            std::cout << result << std::endl;
        }; 
    }
    */

    std::string input = argv[1];
    std::vector<std::string> results = index.getFilenamesBySearchTerm(input);
    for (std::string result : results) {
        std::cout << result << std::endl;
    };

    if (parseFilesThread.joinable()) {
        parseFilesThread.join();
    }
}