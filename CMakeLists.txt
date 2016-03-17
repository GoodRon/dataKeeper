# cmake_minimum_required(VERSION <specify CMake version here>)
project(dataKeeper)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp include/Application.h include/Application.cpp)
add_executable(dataKeeper ${SOURCE_FILES})