#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <sys/inotify.h>
#include <unistd.h>


#ifndef filesystemwatcher_h
#define filesystemwatcher_h

#define EVENT_SIZE          ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN       ( 1024 * ( EVENT_SIZE + 16 ) )

#define EVENT_TYPE_CREATED  "created"
#define EVENT_TYPE_DELETED  "deleted"
#define EVENT_TYPE_MOVED    "moved"


class FileSystemEvent {
    public:
        FileSystemEvent(std::string type, std::string filepath);
        std::string getEventType(void);
        std::string getFilePath(void);
    private:
        std::string type;
        std::string filepath;
};

class FileSystemWatcher {
    public:
        FileSystemWatcher();
        ~FileSystemWatcher();
        FileSystemEvent getFileChangedEvent(void);
        void start(void);
        bool fileChanged(void);
        std::string getWatch(int wd);
        void addEvent(FileSystemEvent event);
        bool shutdown = false;
        int inotify;
    private:
        void addInotifyWatches();
        bool pathInIgnoreList(std::string path);
        std::string targetDir = getenv("HOME");
        std::thread watchThread;
        std::map<int, std::string> watches;
        std::queue<FileSystemEvent> eventQueue;
        std::vector<std::string> ignoreList = {"/.", "/var/lib/snapd/", "/run/", "/dev"};
};

#endif