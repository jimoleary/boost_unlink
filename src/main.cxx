// parse the command line 
// setup the files
// call delete      

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iterator>

#include "util.hpp"
#include "mongo_ops.hpp"

int main (int argc, const char *argv[])
{
    storageGlobalParams.processCmdLine(argc,argv);
    if(setup()  && !storageGlobalParams.onlysetup) {
        DEBUG() << "calling delete" << std::endl;
        _deleteDataFiles(storageGlobalParams.dbname.c_str());
    }
    TRACE() << "exiting" << std::endl;
    return 0;
}
