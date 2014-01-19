// A simple program that computes the square root of a number

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iterator>

#include <boost/regex.hpp>


#include "util.h"
#include "mongo_ops.h"

nullstream logstream;
using namespace std;
using namespace boost::filesystem;

bool checkPath(const path p){
    //
    // we should back off if the directory contains
    //    * name.ns        <- database namespace
    //    * name.0 ...     <- database extent file
    //    * mongod.lock   <- lock file 
    //    * j._0 ..        <- journal file(s)
    static const boost::regex files("^.*(\\.(ns|\\d+)|mongod.lock|j\\._\\d+)$", boost::regex::perl);

    // we should back off if the directory contains
    //    * journal        <- journal directory per db
    //    * local          <- local drectory per db
    static const boost::regex directories("^.*/(journal|local)$", boost::regex::perl);

    INFO() << "checkPath " << p << " exists " << exists(p) << endl;
    if (exists(p)){
        DEBUG() << "checkPath " << p << " is_regular_file " << is_regular_file(p) << endl;
        if(is_regular_file(p)) {
            ERR() << p << " is a file" << endl;
            std::cout << "checkPath(" << p << ") -> returning false" << std::endl;
            return false;
        }
        DEBUG() << "checkPath " << p << " is_directory " << is_directory(p) << endl;
        if(is_directory(p)) {
            directory_iterator end_iter;
            for ( directory_iterator dir_itr( p ); dir_itr != end_iter; ++dir_itr ){
                const char*  name = dir_itr->path().string().c_str();
                DEBUG() << "checkPath " << name 
                        << " is_regular_file " << is_regular_file( dir_itr->status()) 
                        << " match " << boost::regex_match(name, files) << endl;
                if ( (is_regular_file( dir_itr->status() ) && boost::regex_match(name, files)) ||
                     (is_directory( dir_itr->status() ) && boost::regex_match(name, directories))) {
                    ERR() << dir_itr->path() << " exists" << endl;
                    std::cout << "checkPath(" << p << ") -> returning false" << std::endl;
                    return false;
                }
            }
        }
    }    
    std::cout << "checkPath(" << p << ") -> returning true" << std::endl;
    return true;
}
bool touch(const char *name,off_t length){
    off_t offset = 0;
    int flags = O_RDWR;
    struct stat sts;
    int fd;

    INFO() << "name='" << name<< "',length="<< length <<endl;

    if ((stat (name, &sts)) == -1) {
        flags |= O_CREAT;
    } else {
        flags |= O_TRUNC;
    }
    
    fd = ::open(name,flags,S_IRWXU);
    if (fd < 0) {
        perror("Error opening file");
        return false;
    }
    INFO() << "touch " << length << endl;
    if(length != EMPTY){
#if defined(__linux__)
#   if defined(HAVE_POSIX_FALLOCATE)
        return posix_fallocate(fd, offset, length) == 0;
#   else 
        return fallocate(fd, 0, offset, length) == 0;
#   endif
#elif defined(__APPLE__)

        INFO() << "allocating file " <<endl;
        fstore_t store = {F_ALLOCATECONTIG, F_PEOFPOSMODE, 0, length};
        // Try to get a continous chunk of disk space
        int ret = fcntl(fd, F_PREALLOCATE, &store);
        if(-1 == ret){
            // OK, perhaps we are too fragmented, allocate non-continuous
            store.fst_flags = F_ALLOCATEALL;
            ret = fcntl(fd, F_PREALLOCATE, &store);
            if (-1 == ret)
                return false;
        }
        INFO() << "calling truncate " << fd << " " << length <<endl;
        return 0 == ftruncate(fd, length);
#endif
    }
    std::cout << "touch(" << name << ") -> returning true" << std::endl;
    return true;
}

bool setup(StorageGlobalParams &params){
    path p(params.dbpath);

    if(!checkPath(p)){
        std::cout << "checkPath(" << p << ") -> false" << std::endl;
        return false;
    }


    p = p / params.name;
    params.dbpath = p.c_str();
    if (exists(p) && is_regular_file(p)) {
        ERR() << p << "is a file  " << endl;
        return false;
    } else {
        if (!exists(p) && !create_directories(p)) {
            ERR() << "Unable to create " << p << endl;
            return false;
        }
    }

    if (params.directoryperdb) {
        p /= params.dbname;
    }

    if(!checkPath(p)){
        return false;
    }

    string c = params.dbname;
    c += '.';
    
    if (create_directories(p)) {
        TRACE() << "Success created " << p << endl;
    }
    path q;
    q = p / (params.dbname+".namespace");
    TRACE() << "creating " << q << endl;
    touch(q.string().c_str(),  params.preallocate ? MB(16L) : EMPTY);
    TRACE() << "created " << q << endl;
    
    for(int i = 0; i< params.files; i++){
        off_t size = MB(64L);
        int pw = i;
        stringstream ss;
        ss << c << i;
        q = p / (ss.str() + "extent");
        TRACE() << "creating " << q << endl;
        if(i > 4){
            pw = 5;
        }
        size = size * pow(2,pw);
        touch(q.string().c_str(), params.preallocate ? size : EMPTY);
        TRACE() << "created " << q << endl;
    }
    INFO() << "dname " << params.dbname << endl;
    return true;
}
