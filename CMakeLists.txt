cmake_minimum_required(VERSION 2.8)
project(dataKeeper)

set(PROGRAM_NAME "dataKeeper")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-Wall -Wextra)

if(NOT DEFINED LIB_DIR)
    set(LIB_DIR "lib")
endif()

include_directories(include)
include_directories(libMsgPack)

add_subdirectory(libMsgPack)
add_subdirectory(libZmqIpc)

set(SOURCE_FILES main.cpp KeeperApplication.h KeeperApplication.cpp)
add_executable(dataKeeper ${SOURCE_FILES})
