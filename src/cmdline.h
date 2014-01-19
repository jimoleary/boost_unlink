// A simple program that computes the square root of a number
#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

#define MB(x) ((x) * 1024L *1024L)
#define EMPTY -2LL

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/regex.hpp>

class StorageGlobalParams {
public:
    StorageGlobalParams() :
        dbpath("/tmp/boost_unlink/"),
        directoryperdb(false),
        files(1),
        verbose(0),
        dryrun(false),
        onlysetup(false),
        preallocate(false),
        dbname("test"),
        name("_boost_unlink")
    {
    }        
    int files;
    int verbose;
    bool dryrun;
    bool onlysetup;
    bool preallocate;
    std::string dbname;
    std::string dbpath;
    std::string name;
    bool directoryperdb;
    bool processCmdLine(int ac, const char** av, bool &success);
    
};

extern StorageGlobalParams storageGlobalParams;

