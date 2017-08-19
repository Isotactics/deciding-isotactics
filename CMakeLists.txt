cmake_minimum_required(VERSION 3.0)

project (isotactics)

set (CMAKE_BINARY_DIR ../../../bin)
set (CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.50 COMPONENTS graph)


subdirs(iso-dev iso-decision iso-lib)

