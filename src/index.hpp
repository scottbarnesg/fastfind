#include <memory>
#include <vector>
#include <string>
#include <map>
#include <marisa.h>


#ifndef index_h
#define index_h

class Index{
    public:
        void addFiles(std::vector<std::string> filePaths);
        void setFilePathDelimeter(std::string filePathDelimeter);
        std::vector<std::string> getFilenamesBySearchTerm(std::string searchTerm);
    private:
        std::string getFilenameFromFilepath(std::string filePath);
        std::vector<std::string> getIndexKeysFromFilename(std::string filename);
        std::map<std::string, std::vector<std::string>> searchIndex;
        std::string filePathDelimeter = "/";
};

#endif