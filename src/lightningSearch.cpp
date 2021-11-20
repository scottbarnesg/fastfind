#include <iostream>

#include "parser.hpp"
#include "index.hpp"

int main() {
    std::cout << "Parsing files...";
    Parser parser = Parser();
    std::string searchDir = "/";
    parser.findFiles(searchDir); 
    std::cout << " done. Found " << parser.getFilePaths().size() << " files." << std::endl;
    std::cout << "Generating search index...";
    Index index = Index();
    index.addFiles(parser.getFilePaths());
    std::cout << " done." << std::endl;
    index.getFilenamesBySearchTerm("a");
}