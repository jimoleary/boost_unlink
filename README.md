boost_unlink
============

Test code for mongodb drop database

# Setup install dependencies

## Ubuntu 

    >  sudo apt-get install cmake -y 
    >  sudo apt-get install build-essential checkinstall -y 

## Centos 6.3:

    > sudo yum install cmake -y 

## Install boost 1.49.0

    > wget -c 'http://sourceforge.net/projects/boost/files/boost/1.49.0/boost_1_49_0.tar.bz2/download' -O boost_1_49_0.tar.bz2
    > tar xf boost_1_49_0.tar.bz2
    > cd boost_1_49_0/

To install for a single user locally

    > export BOOST_ROOT=$HOME/boost
    > mkdir $BOOST_ROOT
    > ./bootstrap.sh --prefix=$BOOST_ROOT
    > ./b2 install

To install for all users (needs sudo)

    > ./bootstrap.sh
    > sudo ./b2 install

# Build Project

Please ensure you have set BOOST_ROOT in this shell

    > mkdir ./build
    > cmake .. 
    > make 

Compiling 
------

To use Boost_USE_STATIC_LIBS and Boost_USE_STATIC_RUNTIME are set on in the config but you can always add :

    -DBoost_USE_STATIC_LIBS=ON -DBoost_USE_STATIC_RUNTIME=ON

To enable debug use :

    -DBoost_DEBUG=on

Clean the project:

Set the BOOST_ROOT to the correct value.

    > rm -rf ./build && mkdir ./build && cd ./build &&  cmake ..


# Source 

    .
    ├── CMakeLists.txt              # cmake config file
    ├── LICENSE
    ├── README.md
    └── src
        ├── cmdline.cpp             # process the command line options
        ├── main.cpp                # simple mainline wrapper
        ├── util.cpp                # util methods to prime the test
        └── mongo_ops.cpp           # the ops to test 

Getting Help :

    $ ./boost_unlink  --help 
    Allowed options:

    Generic options:
      -h [ --help ]         produce help message
      -v [ --verbose ]      be more verbose (include multiple times for more 
                            verbosity e.g. -vvvvv)
      --dbpath arg          the target directory
      --dryrun              do a dryrun
      --preallocate         preallocate files pf the correct sizes
      --directoryperdb      each database will be stored in a separate directory, 
                            <default to false>)
      --setup               only setup the directory/files, <default to false>)
      -f [ --files ] arg    the number of files

Invoking the executable 
------

    > sudo mkdir /data/boost_unlink
    > sudo chown <username> /data/boost_unlink
    > ./boost_unlink testing --files 6  -vvvvv 
    > ./boost_unlink testing --files 6  -vvvvv --directoryperdb
    > ./boost_unlink testing --files 6   --directoryperdb --dbpath=/data/boost_unlink
    > ./boost_unlink testing --files 6   --dbpath=/data/boost_unlink
    > ls /tmp/boost_unlink/testing
