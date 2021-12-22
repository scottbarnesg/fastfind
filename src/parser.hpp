#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#ifndef parser_h
#define parser_h

class Parser{
    public:
        void findFiles(std::string searchPath);
        std::vector<std::string> getFilePaths(void);
    private:
        std::vector<std::string> filePaths;
};

#endif