cmake_minimum_required(VERSION 3.0)

include_directories(${isotactics_SOURCE_DIR}/iso-lib/include)

file (GLOB SRC_FILES *.cpp)

add_library (iso-lib ${SRC_FILES})
