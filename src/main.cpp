#include <iostream>
#include <filesystem>
#include <thread>

#include "parser.hpp"
#include "index.hpp"

void parse_files() {
    // Parse files on disk
    Parser parser = Parser();
    // std::string searchDir = std::filesystem::current_path();
    std::string searchDir = "/";
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

int main() {
    Index index = Index();
    std::thread parseFilesThread;
    if (index.canLoadFromFile()) {
        // Start background thread to parse files
        std::cout << "Parsing files in background..." << std::endl;
        parseFilesThread = std::thread{parse_files};
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

    if (parseFilesThread.joinable()) {
        parseFilesThread.join();
    }
}