cmake_minimum_required (VERSION 2.6.4)
project (Unlink)
FILE (GLOB SOURCES src/*.cpp)

add_executable(boost_unlink ${SOURCES})

FIND_PACKAGE( Boost 1.49.0 COMPONENTS program_options filesystem system regex REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
TARGET_LINK_LIBRARIES( boost_unlink ${Boost_LIBRARIES} )

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      OFF)
set(Boost_USE_STATIC_RUNTIME     OFF)
