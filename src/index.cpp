#include <iostream>

#include "index.hpp"

void Index::addFiles(std::vector<std::string> filePaths) {
    for (std::string& filePath : filePaths){
        std::string filename = this->getFilenameFromFilepath(filePath);
        // Check if filename exists in the search index. 
        if (this->searchIndex.count(filename) > 0) {
            // If so, get the existing list of filenames and append the new one
            std::vector<std::string> existingEntries = this->searchIndex.find(filename)->second;
            existingEntries.push_back(filePath);
            this->searchIndex[filename] = existingEntries;
        }
        else {
            // Otherwise, create a new vector with this filename and create the index
            std::vector<std::string> newEntry {filePath};
            this->searchIndex.insert(std::pair<std::string, std::vector<std::string>>(filename, newEntry));
        }
    }
}

void Index::setFilePathDelimeter(std::string filePathDelimeter) {
    this->filePathDelimeter = filePathDelimeter;
}

std::vector<std::string> Index::getFilenamesBySearchTerm(std::string searchTerm){
    std::vector<std::string> filePaths;    
    std::map<std::string, std::vector<std::string>>::iterator startIndexIterator = this->searchIndex.lower_bound(searchTerm);
    // TODO: Can we get a map index from this iterator?
    std::map<std::string, std::vector<std::string>>::iterator endIndexIterator = this->searchIndex.upper_bound(searchTerm);
    for (std::map<std::string, std::vector<std::string>>::iterator iter = startIndexIterator; iter != endIndexIterator; ++iter) {
        for (std::string filePath: iter->second) {
            filePaths.push_back(filePath);
        }
    }
    return filePaths;
}

std::string Index::getFilenameFromFilepath(std::string filePath){
    std::size_t last_path_delimiter = filePath.find_last_of(this->filePathDelimeter);
    return filePath.substr(last_path_delimiter + 1);
}

std::vector<std::string> Index::getIndexKeysFromFilename(std::string filename){
    /**
     * Iterates the filename and return all substrings
     **/
    std::vector<std::string> indicies;
    for (int i = 0; i < filename.length(); i++) {
        for (int j = 0; j < i; i++){
            indicies.push_back(filename.substr(j, i));
        }
    }
    return indicies;
}