#include <iostream>

#include "index.hpp"

void Index::addFiles(std::vector<std::string> filePaths) {
    // Create keyset for trie
     marisa::Keyset keyset;
    // Iterate the files, adding them to the searchIndex and keyset
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
            // Add new key to trie
            keyset.push_back(filename);
        }
    }
    // Create Trie
    this->trie.build(keyset);
}

void Index::setFilePathDelimeter(std::string filePathDelimeter) {
    this->filePathDelimeter = filePathDelimeter;
}

std::vector<std::string> Index::getFilenamesBySearchTerm(std::string searchTerm){
    // Get matching keys from trie
    marisa::Agent agent;
    agent.set_query(searchTerm);
    std::vector<std::string> keys;
    while (trie.predictive_search(agent)) {
        std::string newKey(agent.key().ptr(), agent.key().ptr() + agent.key().length());
        if (newKey.find(this->filePathDelimeter) == std::string::npos) {
            keys.push_back(newKey);
        }       
    }
    std::vector<std::string> allFilePaths;
    for (std::string key: keys) {
        std::vector<std::string> filePaths = this->searchIndex.find(key)->second;
        for (std::string filePath: filePaths) {
            allFilePaths.push_back(filePath); 
        }
    }
    return allFilePaths;  
}

std::string Index::getFilenameFromFilepath(std::string filePath){
    std::size_t last_path_delimiter = filePath.find_last_of(this->filePathDelimeter);
    return filePath.substr(last_path_delimiter + 1);
}