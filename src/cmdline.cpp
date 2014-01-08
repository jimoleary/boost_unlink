// Parse the command line args

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iterator>

#include "cmdline.h"

using namespace std;
using namespace boost::program_options;

// Global params 
StorageGlobalParams storageGlobalParams;

int StorageGlobalParams::processCmdLine(int ac, const char** av)
{
    try {
        options_description hidden("Hidden options");
        hidden.add_options()
            ("dbname", value(&dbname), "db name");

        for (string s = "vv"; s.length() <= 12; s.append("v")) {
            hidden.add_options()(s.c_str(), "verbose");
        }
        
        options_description generic("Generic options");
        generic.add_options()
            ("help,h", "produce help message")
            ("verbose,v", "be more verbose (include multiple times for more verbosity e.g. -vvvvv)")
            ("dbpath",value(&storageGlobalParams.dbpath), "the target directory")
            ("dryrun", "do a dryrun")
            ("preallocate", "preallocate files pf the correct sizes")
            ("directoryperdb","each database will be stored in a separate directory, <default to false>)")
            ("setup","only setup the directory/files, <default to false>)")
            ("files,f",value(&files), "the number of files")            
            ;
        int style = (((command_line_style::unix_style ^
                       command_line_style::allow_guessing) |
                      command_line_style::allow_long_disguise) ^
                     command_line_style::allow_sticky);
    
        options_description all;
        all.add(generic);
        all.add(hidden);

        options_description visible("Allowed options");
        visible.add(generic);//.add(config);
        
        positional_options_description positional;
        positional.add("dbname", -1);
        
        variables_map vm;
        
        store(command_line_parser(ac, (char**)av)
              .options(all)
              .positional(positional)
              .style(style)
              .run(), 
              vm);

        notify(vm);
        cout.setf(std::ios::boolalpha);

        if (vm.count("help")) {
            cout << visible << "\n";
            return 0;
        }

        for (string s = "vv"; s.length() <= 12; s.append("v")) {
            if (vm.count(s)) {
                verbose = s.length();
            }
        }
        
        if(vm.count("verbose")){
            verbose = 1;
        }

        if(vm.count("dryrun")){
            dryrun = true;
        }

        if (vm.count("dbname")){
            cout << "dbname is: " << vm["dbname"].as<string>() << "\n";
        }
        if (vm.count("directoryperdb")) {
            storageGlobalParams.directoryperdb = true;
        }
        if (vm.count("setup")) {
            onlysetup = true;
        }
        if (vm.count("preallocate")) {
            preallocate = true;
        }
        cout << "directoryperdb " << storageGlobalParams.directoryperdb << endl;
    }
    catch(exception& e){
        cout << e.what() << "\n";
        return 1;
    }    
    return 0;
}
