cmake_minimum_required(VERSION 2.8)
project(dataKeeper)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
add_definitions(-Wall -Wextra -Wno-unknown-pragmas)

# Указывает, какие плагины БД используются
set(DB_PLUGIN_LIST pgsql sqlite )
# set(DB_PLUGIN_LIST pgsql sqlite mysql)


if(NOT DEFINED SHARED_LIB_DIR)
    set(SHARED_LIB_DIR "lib")
endif()

if(NOT DEFINED LIB_DIR)
    set(LIB_DIR "lib")
endif()

if(NOT DEFINED BIN_DIR)
    set(BIN_DIR "bin")
endif()

include_directories(.)
include_directories(include)
include_directories(plugins)
include_directories(plugins/messages)
include_directories(plugins/journal)

add_subdirectory(plugins)

add_subdirectory(tests)

set(SOURCE_FILES main.cpp KeeperApplication.cpp)
add_executable(dataKeeper ${SOURCE_FILES})
target_link_libraries(dataKeeper jsoncpp dl ZmqIpc libMsgPack)

install(TARGETS dataKeeper RUNTIME DESTINATION ${BIN_DIR})
