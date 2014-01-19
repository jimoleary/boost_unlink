// parse the command line 
// setup the files
// call delete      

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iterator>

#include "util.h"
#include "mongo_ops.h"

int main (int argc, const char *argv[])
{
    bool success = true;
    if(storageGlobalParams.processCmdLine(argc,argv, success)) {
        success = setup() ;
        if(success && !storageGlobalParams.onlysetup) {
            DEBUG() << "calling delete" << std::endl;
            _deleteDataFiles(storageGlobalParams.dbname.c_str());
        }
    }
    DEBUG() << "exiting (" << success << ") " << (success ? 0 : 1) << std::endl;
    return success ? 0 : 1;
}
