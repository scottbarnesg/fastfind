#include <chrono>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <thread>

#ifndef lock_h
#define loch_h

class Lockfile {
    public:
        Lockfile();
        bool isLocked();
        bool hasLock();
        void getLock();
        void releaseLock();
        void waitForLock();
    private:
        std::string lockFileDir = "data/.lockfile/";
        std::string lockFile = this->lockFileDir + "lockfile";
        bool ownsLock = false;
};

#endif