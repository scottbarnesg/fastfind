#include <queue>
#include <stdexcept>
#include <string>
#include <thread>
#include <sys/inotify.h>
#include <unistd.h>

#ifndef filesystemwatcher_h
#define filesystemwatcher_h

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

#define EVENT_TYPE_CREATED "created"
#define EVENT_TYPE_DELETED "deleted"

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
        void addEvent(FileSystemEvent event);
        bool shutdown = false;
        int inotify, watch;
    private:
        std::string targetDir = "/";
        std::thread watchThread;
        std::queue<FileSystemEvent> eventQueue;
};

#endif