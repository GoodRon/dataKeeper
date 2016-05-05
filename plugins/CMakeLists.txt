cmake_minimum_required(VERSION 2.8)

add_subdirectory(messages)
add_subdirectory(journal)

add_library(jsonConfigHelper STATIC jsonConfigHelper.cpp)
target_link_libraries(jsonConfigHelper jsoncpp)
target_compile_options(jsonConfigHelper
        PRIVATE
        -Wall -Wextra -pedantic
        -fno-strict-aliasing -std=c++0x -fPIC)

