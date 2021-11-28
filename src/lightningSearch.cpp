#include <iostream>
#include <filesystem>

#include "parser.hpp"
#include "index.hpp"

int main() {
    Parser parser = Parser();
    std::string searchDir = std::filesystem::current_path();
    std::cout << "Parsing files in " << searchDir << " ..." << std::flush;;
    parser.findFiles(searchDir); 
    std::cout << " done. Found " << parser.getFilePaths().size() << " files." << std::endl;
    std::cout << "Generating search index..." << std::flush;;
    Index index = Index();
    index.addFiles(parser.getFilePaths());
    std::cout << " done." << std::endl;
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
}