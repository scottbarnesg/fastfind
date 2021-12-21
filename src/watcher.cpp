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
    printf("Started file watch thread.\n");
    while(true) {
        printf("entered loop");
        int i = 0;
        printf("Creating buffer...");
        char buffer[EVENT_BUF_LEN];
        printf("Reading from buffer...");
        int length = read(watcher->inotify, buffer, EVENT_BUF_LEN);
        printf("done.\n");
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            if (event->len) {
                printf("Event detected for %s .\n", event->name);
                if (event->mask & IN_CREATE) {
                    FileSystemEvent createEvent = FileSystemEvent(EVENT_TYPE_CREATED, event->name);
                    watcher->addEvent(createEvent);
                    printf("The file %s was created.\n", event->name);

                } else if (event->mask & IN_DELETE) {
                    FileSystemEvent deleteEvent = FileSystemEvent(EVENT_TYPE_DELETED, event->name);
                    watcher->addEvent(deleteEvent);
                    printf("The file %s was deleted.\n", event->name);
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
    this->watch = inotify_add_watch(this->inotify, this->targetDir.c_str(), IN_CREATE | IN_DELETE);
    if (this->watch < 0) {
        throw std::runtime_error("Failed to add watch on: " + this->targetDir);
    }
}

FileSystemWatcher::~FileSystemWatcher() {
    shutdown = true;
    inotify_rm_watch(this->inotify, this->watch);
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

FileSystemEvent FileSystemWatcher::getFileChangedEvent() {
    FileSystemEvent event = this->eventQueue.front();
    this->eventQueue.pop();
    return event;
}