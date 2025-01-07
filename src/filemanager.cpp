#include "filemanager.h"
#include <stdexcept>

FileManager::FileManager(const std::string& path, int mode) : file(nullptr) {
    file = sf_open(path.c_str(), mode, &sfinfo);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }
}

FileManager::FileManager(const std::string& path, int mode, const SF_INFO& sfinfo)
    : file(nullptr), sfinfo(sfinfo) {
    file = sf_open(path.c_str(), mode, &this->sfinfo);
    if (!file) {
        throw std::runtime_error("Failed to open output file: " + path);
    }
}

FileManager::~FileManager() {
    if (file) sf_close(file);
}

SNDFILE* FileManager::getFile() const {
    return file;
}

SF_INFO FileManager::getSFInfo() const {
    return sfinfo;
}