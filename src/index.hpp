#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

#include <marisa.h>

#include "files.hpp"


#ifndef index_h
#define index_h

class Index{
    public:
        void addFiles(std::vector<std::string> filePaths);
        void setFilePathDelimeter(std::string filePathDelimeter);
        bool canLoadFromFile();
        void saveToDisk();
        void loadFromDisk();
        std::vector<std::string> getFilenamesBySearchTerm(std::string searchTerm);
    private:
        FileManager fileManager = FileManager();
        std::string getFilenameFromFilepath(std::string filePath);
        void saveTrieToFile(std::string filename);
        void loadTrieFromFile(std::string filename);
        void saveMapToFile(std::string filename);
        void loadMapFromFile(std::string filename);
        void createDataDir(void);
        std::map<std::string, std::vector<std::string>> searchIndex;
        marisa::Trie trie;
        std::string filePathDelimeter = "/";
        std::string dataDir = "data/";
        std::string mapFilePath = "map.csv";
        std::string trieFilePath = "trie.bin";
};

#endif