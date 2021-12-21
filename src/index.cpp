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

bool Index::canLoadFromFile() {
    std::filesystem::path trieFile(this->trieFilePath), mapFile(this->mapFilePath);
    return std::filesystem::exists(trieFile) && std::filesystem::exists(mapFile);
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
        try {
            std::vector<std::string> filePaths = this->searchIndex.find(key)->second;
            for (std::string filePath: filePaths) {
                allFilePaths.push_back(filePath); 
            }
        } catch (const std::bad_array_new_length& e) {
            std::cerr << e.what() << std::endl;
            std::cerr << "Skipping files for search key: " << key << std::endl;
            continue;
        }
    }
    return allFilePaths;  
}

void Index::saveTrieToFile(std::string filename){
    this->createDataDir();
    this->trie.save(filename.c_str());
}

void Index::loadTrieFromFile(std::string filename) {
    this->trie.load(filename.c_str());
}

void Index::saveMapToFile(std::string filename) {
    this->createDataDir();
    this->fileManager.writeMapToFile(filename, this->searchIndex);
}

void Index::loadMapFromFile(std::string filename) {
    this->searchIndex = this->fileManager.readMapFromFile(filename);
}

void Index::createDataDir(){
    if (!std::filesystem::exists(this->dataDir)) {
        std::filesystem::create_directory(this->dataDir);
    }
}

void Index::saveToDisk(){
    this->saveTrieToFile(this->trieFilePath);
    this->saveMapToFile(this->mapFilePath);
}

void Index::loadFromDisk(){
    this->loadTrieFromFile(this->trieFilePath);
    this->loadMapFromFile(this->mapFilePath);
}

std::string Index::getFilenameFromFilepath(std::string filePath){
    std::size_t last_path_delimiter = filePath.find_last_of(this->filePathDelimeter);
    return filePath.substr(last_path_delimiter + 1);
}