#include <iostream>

#include "watcher.hpp"

int main() {
    // Set up the file system watcher
    FileSystemWatcher watcher = FileSystemWatcher();
    watcher.start();
    // Check for events
    while (true) {
        if (watcher.fileChanged()) {
            FileSystemEvent event = watcher.getFileChangedEvent();
            std::cout << event.getEventType() << ": " << event.getFilePath() << std::endl;
        }
        else {
            sleep(1);
        }
    }
}