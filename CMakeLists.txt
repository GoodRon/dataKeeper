cmake_minimum_required(VERSION 2.8)
project(dataKeeper)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-Wall -Wextra)

if(NOT DEFINED LIB_DIR)
    set(LIB_DIR "lib")
endif()

if(NOT DEFINED BIN_DIR)
    set(BIN_DIR "bin")
endif()

include_directories(include)
include_directories(libMsgPack)
include_directories(libZmqIpc/include)
include_directories(libZmqIpc/src)

#add_subdirectory(libMsgPack)
#add_subdirectory(libZmqIpc)

add_subdirectory(tests)

#set(SOURCE_FILES main.cpp KeeperApplication.h KeeperApplication.cpp AbstractDatabase.cpp AbstractDatabase.h include/config.h)
#add_executable(dataKeeper ${SOURCE_FILES})
#target_link_libraries(dataKeeper ZmqIpc)

#install(TARGETS dataKeeper RUNTIME DESTINATION ${BIN_DIR})