boost_unlink
============

Test code for mongodb drop database

# Ubuntu 

   >  sudo apt-get install cmake -y 
   >  sudo apt-get install build-essential checkinstall -y 

Centos 6.3:

   > sudo yum install cmake -y 

# Install boost 1.49.0

    > mkdir boost 
    > cd boost
    > wget -c 'http://sourceforge.net/projects/boost/files/boost/1.49.0/boost_1_49_0.tar.bz2/download'
    > tar xf boost_1_49_0.tar.bz2
    > cd boost_1_49_0/
    > ./bootstrap.sh --prefix=~/boost
    > ./bootstrap.sh
    > ./b2 install

# Build exe


    > cd unlink_test/build
    > BOOST_ROOT=/usr/local cmake .. 
    > BOOST_ROOT=~/boost cmake .. 
    > make 
    > sudo mkdir /data/boost_unlink
    > sudo chown <username> /data/boost_unlink
    > ./boost_unlink testing --files 100  -vvvvv 
    > ./boost_unlink testing --files 100  -vvvvv --directoryperdb
    > ./boost_unlink testing --files 100   --directoryperdb --dbpath=/data/boost_unlink
    > ./boost_unlink testing --files 100   --dbpath=/data/boost_unlink
    > ls /tmp/boost_unlink/testing

Boost_USE_STATIC_LIBS and Boost_USE_STATIC_RUNTIME are set on in the config but you can always add :

    -DBoost_USE_STATIC_LIBS=ON -DBoost_USE_STATIC_RUNTIME=ON

To enable debug use :

    -DBoost_DEBUG=on

Clean the project:

    > rm -rf ./build && mkdir ./build && cd ./build &&  BOOST_ROOT=/usr/local cmake ..


Source 

    .
    ├── CMakeLists.txt              # cmake config file
    ├── LICENSE
    ├── README.md
    └── src
        ├── cmdline.cxx             # process the command line options
        ├── main.cxx                # simple mainline wrapper
        ├── util.cxx                # util methods to prime the test
        └── mongo_ops.cxx           # the ops to test 

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

