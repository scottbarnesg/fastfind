#include <iostream>

#include "index.hpp"
#include "parser.hpp"
#include "watcher.hpp"


void parse_files() {
    // Parse files on disk
    Parser parser = Parser();
    // std::string searchDir = std::filesystem::current_path();
    std::string searchDir = "/home/scottgbarnes/";
    std::cout << "Parsing files in " << searchDir << " ..." << std::flush;;
    parser.findFiles(searchDir); 
    std::cout << " done. Found " << parser.getFilePaths().size() << " files." << std::endl;
    // Generate search index
    std::cout << "Generating search index..." << std::flush;
    Index index = Index();
    index.addFiles(parser.getFilePaths());
    std::cout << " done." << std::endl;
    // Write files to disk
    index.saveToDisk();
}

void updateIndex(FileSystemEvent event) {
    Index index = Index();
    index.loadFromDisk();
    index.buildKeyset();
    if (event.getEventType() == EVENT_TYPE_CREATED) {
        index.addFile(event.getFilePath());
        index.buildIndex();
        index.saveToDisk();
    }
    else if (event.getEventType() == EVENT_TYPE_DELETED) {
        index.removeFile(event.getFilePath());
        index.buildIndex();
        index.saveToDisk();
    }
    // TODO: Add handling for EVENT_TYPE_MOVE
}

int main() {
    // Run parser to get initial state
    parse_files();
    // Set up the file system watcher
    FileSystemWatcher watcher = FileSystemWatcher();
    watcher.start();
    // Check for events
    while (true) {
        if (watcher.fileChanged()) {
            FileSystemEvent event = watcher.getFileChangedEvent();
            std::cout << event.getEventType() << ": " << event.getFilePath() << std::endl;
            // TODO: Need some sort of mechanism to only update the index every N seconds.
            updateIndex(event);
        }
        else {
            sleep(1);
        }
    }
}