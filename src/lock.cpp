#include "lock.hpp"

Lockfile::Lockfile() {
    std::filesystem::path lockfileDir{this->lockFileDir};
    if (!std::filesystem::exists(lockfileDir)) {
        std::filesystem::create_directory(this->lockFileDir);
    }
}


bool Lockfile::isLocked() {
    std::filesystem::path filePath{this->lockFile};
    return std::filesystem::exists(filePath);
}

bool Lockfile::hasLock() {
    return this->ownsLock;
}

void Lockfile::getLock() {
    if (this->isLocked()) {
        throw std::runtime_error("Failed to acquire lock.");
    }
    std::ofstream output(this->lockFile);
    this->ownsLock = true;
}

void Lockfile::releaseLock() {
    if (!this->isLocked()) {
        throw std::runtime_error("Could not release lock, it does not exist.");
    }
    std::filesystem::remove(this->lockFile);
    this->ownsLock = false;
}

void Lockfile::waitForLock() {
    while(this->isLocked()) {
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
    }
    this->getLock();
}