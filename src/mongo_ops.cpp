// There are the ops we want to test 

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iterator>

#include "util.h"
#include "mongo_ops.h"

using namespace std;
namespace fs = boost::filesystem;

FileAllocator* FileAllocator::instance =new FileAllocator();

void _applyOpToDataFiles( const char *database, FileOp &fo, bool afterAllocator, const string& path = storageGlobalParams.dbpath) {
    if ( afterAllocator )
        FileAllocator::get()->waitUntilFinished();
    string c = database;
    c += '.';
    fs::path p(path);
    if (storageGlobalParams.directoryperdb)
        p = p / database;
    fs::path q;
    q = p / (c+"namespace");
    bool ok = false;
    MONGO_ASSERT_ON_EXCEPTION( ok = fo.apply( q ) );
    if ( ok ) {
        LOG(2) << fo.op() << " file " << q.string() << endl;
    }
    int i = 0;
    int extra = 10; // should not be necessary, this is defensive in case there are missing files
    while ( 1 ) {
        verify( i <= DiskLoc::MaxFiles );
        stringstream ss;
        ss << c << i;
        q = p / (ss.str()  + "extent");

        MONGO_ASSERT_ON_EXCEPTION( ok = fo.apply(q) );
        if ( ok ) {
            if ( extra != 10 ) {
                LOG(1) << fo.op() << " file " << q.string() << endl;
            }
        }
        else if ( --extra <= 0 )
            break;
        i++;
    }
}

void _deleteDataFiles(const char *database) {
    if (storageGlobalParams.directoryperdb) {
        fs::path p(fs::path(storageGlobalParams.dbpath) / database);

        if (!storageGlobalParams.dryrun) {
            int removed = remove_all(p);
            DEBUG() << " removed : " << removed << " files "  << endl;
        }
        return;
    }
    class : public FileOp {
        virtual bool apply( const fs::path &p ) {
            if(storageGlobalParams.dryrun) {
                DEBUG() << "FileOp::apply " << op() << p<< endl;
                return true;
            }
            bool val = remove( p);
            DEBUG() << "FileOp::apply " << op() << " " << p << " returned " << val << endl;
            return val;
        }
        virtual const char * op() const {
            return "remove";
        }
    } deleter;
    _applyOpToDataFiles( database, deleter, true );
}
