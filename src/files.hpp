#include <algorithm>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <sstream>
#include <vector>

#ifndef files_h
#define files_h


class FileManager {
    public:
        void writeMapToFile(std::string filename, std::map<std::string, std::vector<std::string>> data);
        std::map<std::string, std::vector<std::string>> readMapFromFile(std::string filename);
    private:
        std::vector<std::string> splitStringBySubstring(std::string target, std::string delimeter);
        std::string delimeter = " , ";
        std::mutex fileLock;
};

#endif