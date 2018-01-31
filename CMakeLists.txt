cmake_minimum_required(VERSION 3.0)

project (isotactics)

# macOS-specific: add this include directory so that
# cmake/Xcode may find Boost installed via HomeBrew.
# see docs/xcode.md for further instructions on using Xcode.
include_directories(/usr/local/include)

set (CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set (CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra")

# setting ISO_LIB_ENABLE_DEBUG_OUTPUT=0 will disable all debug outputs from iso-lib.
# setting this to 1 will enable them. this is a preprocessor option. the
# application has to be recompiled for this change to take effect
add_definitions (-D ISO_LIB_ENABLE_DEBUG_OUTPUT=0)

set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_RUNTIME OFF)
find_package (Boost 1.50 COMPONENTS graph)

subdirs (iso-dev iso-decision iso-lib)
