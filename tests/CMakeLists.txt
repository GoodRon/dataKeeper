cmake_minimum_required(VERSION 2.8)

add_executable(requestTest requestTest.cpp)
target_link_libraries(requestTest ZmqIpc libMsgPack)

add_executable(selectMsgTest selectMsgTest.cpp)
target_link_libraries(selectMsgTest ZmqIpc libMsgPack)