#include <iostream>

#include "parser.hpp"
#include "index.hpp"

int main() {
    Parser parser = Parser();
    std::string searchDir = "/home/scottgbarnes/Documents";
    std::cout << "Parsing files in " << searchDir << " ..." << std::flush;;
    parser.findFiles(searchDir); 
    std::cout << " done. Found " << parser.getFilePaths().size() << " files." << std::endl;
    std::cout << "Generating search index..." << std::flush;;
    Index index = Index();
    index.addFiles(parser.getFilePaths());
    std::cout << " done." << std::endl;
    std::vector<std::string> results = index.getFilenamesBySearchTerm("tes");
    std::cout << "Results: " << std::endl;
    for (std::string result : results) {
        std::cout << result << std::endl;
    }; 
}