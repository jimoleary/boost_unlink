// A simple program that computes the square root of a number
#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include "cmdline.hpp" 

class DiskLoc {
public:
    enum SentinelValues {
        MaxFiles=16000
    };
};

class FileAllocator {
    static FileAllocator *instance;
public:
    static FileAllocator *get() {
        return instance;
    }
    void waitUntilFinished(){}
};

class FileOp {
public:
    virtual ~FileOp() {}
    // Return true if file exists and operation successful
    virtual bool apply( const boost::filesystem::path &p ) = 0;
    virtual const char * op() const = 0;
};

extern void _deleteDataFiles(const char *database) ;
