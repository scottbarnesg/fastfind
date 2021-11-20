#include <iostream>

#include "index.hpp"

void Index::addFiles(std::vector<std::string> filePaths) {
    for (std::string& filePath : filePaths){
        std::string filename = this->getFilenameFromFilepath(filePath);
        std::shared_ptr<std::string> pFilename = std::make_shared<std::string>(filename);
        // std::vector<std::string> keys = this->getIndexKeysFromFilename(filename);
        std::vector<std::string> keys{filename};
        for (std::string& key: keys){
            // TODO: This slams the stack until we run out of memory. Need to figure out a more intelligent way to handle this.
            // Check if key exists in the search index. 
            if (this->searchIndex.count(key) > 0) {
                // If so, get the existing list of filenames and append the new one
                std::vector<std::shared_ptr<std::string>> existingEntries = this->searchIndex.find(key)->second;
                existingEntries.push_back(pFilename);
                this->searchIndex.insert(std::pair<std::string, std::vector<std::shared_ptr<std::string>>>(key, existingEntries));
            }
            else {
                // Otherwise, create a new vector with this filename and create the index
                std::vector<std::shared_ptr<std::string>> newEntry {pFilename};
                this->searchIndex.insert(std::pair<std::string, std::vector<std::shared_ptr<std::string>>>(key, newEntry));
            }
        }
    }
}

std::vector<std::string> Index::getFilenamesBySearchTerm(std::string searchTerm){
    std::vector<std::string> filePaths;    
    std::map<std::string, std::vector<std::shared_ptr<std::string>>>::iterator startIndexIterator = this->searchIndex.lower_bound(searchTerm);
    // TODO: Can we get a map index from this iterator?
    std::map<std::string, std::vector<std::shared_ptr<std::string>>>::iterator endIndexIterator = this->searchIndex.upper_bound(searchTerm);
    for (std::map<std::string, std::vector<std::shared_ptr<std::string>>>::iterator iter = startIndexIterator; iter != endIndexIterator; ++iter) {
        std::cout << iter->first << std::endl;
        for (std::shared_ptr<std::string> pFilePath: iter->second) {
            filePaths.push_back(*pFilePath);
        }
    }
    return filePaths;
}

std::string Index::getFilenameFromFilepath(std::string filePath){
    std::size_t last_path_delimiter = filePath.find_last_of(this->file_path_delimeter);
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