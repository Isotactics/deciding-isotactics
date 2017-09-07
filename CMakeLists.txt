cmake_minimum_required(VERSION 3.0)

project (isotactics)

# macOS-specific: add this include directory so that
# cmake/Xcode may find Boost installed via HomeBrew.
# create new Xcode project by running the following
# command in this directory:
# cmake -G Xcode CMAKE_BUILD_TYPE="Debug" ./
include_directories(/usr/local/include)

set (CMAKE_BINARY_DIR ../../../bin)
set (CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.50 COMPONENTS graph)


subdirs(iso-dev iso-decision iso-lib)
