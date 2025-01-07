#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <sndfile.h>
#include <string>
#include <stdexcept>

class FileManager {
    SNDFILE* file;
    SF_INFO sfinfo;

public:
    //for input
    FileManager(const std::string& path, int mode);
    //for output
    FileManager(const std::string& path, int mode, const SF_INFO& sfinfo);

    ~FileManager();

    SNDFILE* getFile() const;
    SF_INFO getSFInfo() const;
};

#endif // FILEMANAGER_H
