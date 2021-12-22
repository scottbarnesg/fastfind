#include "watcher.hpp"


FileSystemEvent::FileSystemEvent(std::string type, std::string filepath) {
    this->type = type;
    this->filepath = filepath;
}

std::string FileSystemEvent::getEventType() {
    return this->type;
}

std::string FileSystemEvent::getFilePath() {
    return this->filepath;
}


void watchForChanges(FileSystemWatcher *watcher) {
    std::cout << "Started file watch thread." << std::endl;
    while(true) {
        int i = 0;
        char buffer[EVENT_BUF_LEN];
        int length = read(watcher->inotify, buffer, EVENT_BUF_LEN);
        if (length < 0) {
            std::cerr << "Error reading inotify event.\n" << std::endl;
            continue;
        }
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            if (event->len) {
                if (event->mask & (IN_CREATE | IN_MOVED_TO)) {
                    std::string filePath = watcher->getWatch(event->wd) + event->name;
                    FileSystemEvent createEvent = FileSystemEvent(EVENT_TYPE_CREATED, filePath);
                    watcher->addEvent(createEvent);

                }
                else if (event->mask & (IN_DELETE | IN_MOVED_FROM)) {
                    std::string filePath = watcher->getWatch(event->wd) + event->name;
                    FileSystemEvent deleteEvent = FileSystemEvent(EVENT_TYPE_DELETED, filePath);
                    watcher->addEvent(deleteEvent);
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }
}


FileSystemWatcher::FileSystemWatcher() {
    this->inotify = inotify_init();
    if (this->inotify < 0) {
        throw std::runtime_error("Failed to initialize inotify");
    }
    // Recursively add watches to each subdirectory in the target dir
    this->addInotifyWatches();
}

FileSystemWatcher::~FileSystemWatcher() {
    shutdown = true;
    for (std::pair<int, std::string> watch: this->watches) {
        inotify_rm_watch(this->inotify, watch.first);
    }
}

void FileSystemWatcher::start() {
    watchThread = std::thread{watchForChanges, this};
}

void FileSystemWatcher::addEvent(FileSystemEvent event) {
    this->eventQueue.push(event);
}

bool FileSystemWatcher::fileChanged() {
    return !this->eventQueue.empty();
}

std::string FileSystemWatcher::getWatch(int wd){
    return this->watches[wd];
}

FileSystemEvent FileSystemWatcher::getFileChangedEvent() {
    FileSystemEvent event = this->eventQueue.front();
    this->eventQueue.pop();
    return event;
}

void FileSystemWatcher::addInotifyWatches() {
    // Add target directory
    int watch = inotify_add_watch(this->inotify, this->targetDir.c_str(), IN_CREATE | IN_DELETE | IN_MOVE);
    if (watch < 0) {
        throw std::runtime_error("Failed to add watch on: " + this->targetDir);
    }
    this->watches[watch] = this->targetDir + "/";
    // Recursively add subdirectories
    for (const auto& dir : std::filesystem::recursive_directory_iterator(this->targetDir, std::filesystem::directory_options::skip_permission_denied)) {
        try {
            if(std::filesystem::is_directory(dir.path()) && !this->pathInIgnoreList(dir.path())) { 
                watch = inotify_add_watch(this->inotify, dir.path().c_str(), IN_CREATE | IN_DELETE | IN_MOVE);
                if (watch < 0) {
                    continue;
                }
                this->watches[watch] = std::string(dir.path()) + "/";
            }
        } catch (std::filesystem::filesystem_error &e) {
            continue;
        }
    }
}

bool FileSystemWatcher::pathInIgnoreList(std::string path) {
    for (std::string ignoreItem : this->ignoreList) {
        if (path.find(ignoreItem) != std::string::npos){
            return true;
        }
    }
    return false;
}