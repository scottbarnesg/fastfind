#include "parser.hpp"

void Parser::findFiles(std::string searchDir){
    for (const auto& dir : std::filesystem::recursive_directory_iterator(searchDir, 
            std::filesystem::directory_options::skip_permission_denied)) {
        this->filePaths.push_back(dir.path());
    }    
}

std::vector<std::string> Parser::getFilePaths() {
    return this->filePaths;
}