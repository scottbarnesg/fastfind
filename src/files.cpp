#include "files.hpp"

void FileManager::writeMapToFile(std::string filename, std::map<std::string, std::vector<std::string>> data) {
    // Open file for writing, discard and existing data
    this->fileLock.lock();
    std::fstream fout;
    fout.open(filename, std::ios::out | std::ios::trunc);
    // Iterate the map and write data in a csv-ish format
    for (auto const& [key, dataVector] : data) {
        fout << key << delimeter;
        for (auto value: dataVector) {
            fout << value << delimeter;
        }
        fout << "\n";
    }
    this->fileLock.unlock();
}

std::map<std::string, std::vector<std::string>> FileManager::readMapFromFile(std::string filename) {
    std::map<std::string, std::vector<std::string>> data;
    // Open file for reading
    this->fileLock.lock();
    std::fstream fin;
    fin.open(filename, std::ios::in);
    // Read each line, splitting by the delimeter.
    std::string line;
    while (getline(fin, line)) {
        // Split the line by the delimter
        std::vector<std::string> result = this->splitStringBySubstring(line, this->delimeter);
        // Pop key from front of result
        std::string key = result.front();
        result.erase(result.begin());
        // Populate the map
        data[key] = result;
    }
    this->fileLock.unlock();
    return data;
}

std::vector<std::string> FileManager::splitStringBySubstring(std::string target, std::string delimeter) {
    std::vector<std::string> splitString;
    size_t pos = 0;
    while ((pos = target.find(delimeter)) != std::string::npos) {
        splitString.push_back(target.substr(0, pos));
        target.erase(0, pos + delimeter.length());
    }
    return splitString;
}