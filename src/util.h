// A simple program that computes the square root of a number
#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <iostream>


#if defined __linux__ 
#include <linux/falloc.h>
#endif
#include "cmdline.h"

#define MB(x) ((x) * 1024L *1024L)
#define EMPTY -2LL

struct nullstream : std::ostream {
    nullstream() : std::ios(0), std::ostream(0) {}
};
extern nullstream logstream;

#define  msgasserted(msgid, status)
#define  massert(msgid, status, b) 

#define MONGO_ASSERT_ON_EXCEPTION( expression ) \
    try {                                       \
        expression;                             \
    } catch ( const std::exception &e ) {       \
        stringstream ss;                                                \
        ss << "caught exception: " << e.what() << ' ' << __FILE__ << ' ' << __LINE__; \
        msgasserted( 13294 , ss.str() );                              \
    } catch ( ... ) {                                                   \
        massert( 10437 ,  "unknown exception" , false );              \
    }

#define MAX_FILES 16 * 1024
#define LOG(x) ((x < storageGlobalParams.verbose) ? std::cout : logstream)

#define _TRACE 5
#define _DEBUG 4
#define _INFO 3
#define _WARN 2
#define _ERR 1

#define TRACE() ((_TRACE < storageGlobalParams.verbose) ? std::cout << "TRACE : " <<  __FUNCTION__ << " " : logstream)
#define DEBUG() ((_DEBUG < storageGlobalParams.verbose) ? std::cout << "DEBUG : " << __FUNCTION__  << " " : logstream)
#define INFO() ((_INFO < storageGlobalParams.verbose)   ? std::cout << "INFO  : " << __FUNCTION__  << " " : logstream)
#define WARN(x) ((_WARN < storageGlobalParams.verbose)  ? std::cout << "WARN  : " << __FUNCTION__  << " " : logstream)
#define ERR(x)  (std::cout << "ERR   ")


#define verify(x) if(!(x)){std::cout << "verify failed"<<endl;}

/**
 * setup the test dir structure
 * 
 */
bool setup(StorageGlobalParams &params=storageGlobalParams);
