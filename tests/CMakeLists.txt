cmake_minimum_required(VERSION 2.8)

#list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/")
#
#find_package(ODB REQUIRED OPTIONAL_COMPONENTS sqlite pgsql mysql)
#
#include(${ODB_USE_FILE})
#
#if(ODB_SQLITE_FOUND AND NOT ODB_PGSQL_FOUND AND NOT ODB_MYSQL_FOUND)
#    message(FATAL_ERROR "Neither mysql nor sqlite and nor pqsql odb module was found")
#endif()
#
## SQLITE
#
#set(TEST_SOURCES
#        database_plugin.cpp)
#
#set(TEST_ODB_HEADERS
#        message.hxx)
#
#set(TEST_DEFS)
#set(TEST_DBS)
#
##if(ODB_PGSQL_FOUND)
##    list(APPEND TEST_DEFS DATABASE_PGSQL)
##    list(APPEND TEST_DBS pgsql)
##endif()
#
##if(ODB_MYSQL_FOUND)
##    list(APPEND TEST_DEFS DATABASE_MYSQL)
##    list(APPEND TEST_DBS mysql)
##endif()
#
#if(ODB_SQLITE_FOUND)
#    list(APPEND TEST_DEFS DATABASE_SQLITE)
#    list(APPEND TEST_DBS sqlite)
#endif()
#
#odb_compile(TEST_SOURCES
#        FILES ${TEST_ODB_HEADERS}
#        MULTI_DATABASE dynamic
#        DB ${TEST_DBS}
#        GENERATE_QUERY
#        GENERATE_SCHEMA
#        SCHEMA_FORMAT embedded
#        STANDARD c++11)
#
#add_library(libdbplugin_sqlite SHARED ${TEST_SOURCES})
#
#target_link_libraries(libdbplugin_sqlite
#        odb odb-sqlite)
##        ${ODB_LIBRARIES})
#
#target_include_directories(libdbplugin_sqlite SYSTEM
#        PRIVATE
#        ${ODB_INCLUDE_DIRS}
#        ${ODB_COMPILE_OUTPUT_DIR})
#
#target_compile_options(libdbplugin_sqlite
#        PRIVATE
#        -Wall -Wextra -pedantic
#        -fno-strict-aliasing -std=gnu++11 -fPIC)
#
#target_compile_definitions(libdbplugin_sqlite
#        PRIVATE ${TEST_DEFS})
#
#set_target_properties(libdbplugin_sqlite PROPERTIES
#        PREFIX "")
#
## PGSQL
#
#set(TEST_SOURCES
#        database_plugin.cpp)
#
#set(TEST_ODB_HEADERS
#        message.hxx)
#
#set(TEST_DEFS)
#set(TEST_DBS)
#
#if(ODB_PGSQL_FOUND)
#    list(APPEND TEST_DEFS DATABASE_PGSQL)
#    list(APPEND TEST_DBS pgsql)
#endif()
#
#odb_compile(TEST_SOURCES
#        FILES ${TEST_ODB_HEADERS}
#        MULTI_DATABASE dynamic
#        DB ${TEST_DBS}
#        GENERATE_QUERY
#        GENERATE_SCHEMA
#        SCHEMA_FORMAT sql
#        STANDARD c++11)
#
#add_library(libdbplugin_pgsql SHARED ${TEST_SOURCES})
#
#target_link_libraries(libdbplugin_pgsql
#        odb odb-pgsql)
#
#target_include_directories(libdbplugin_pgsql SYSTEM
#        PRIVATE
#        ${ODB_INCLUDE_DIRS}
#        ${ODB_COMPILE_OUTPUT_DIR})
#
#target_compile_options(libdbplugin_pgsql
#        PRIVATE
#        -Wall -Wextra -pedantic
#        -fno-strict-aliasing -std=gnu++11 -fPIC)
#
#target_compile_definitions(libdbplugin_pgsql
#        PRIVATE ${TEST_DEFS})
#
#set_target_properties(libdbplugin_pgsql PROPERTIES
#        PREFIX "")
#
## MYSQL
#
#set(TEST_SOURCES
#        database_plugin.cpp)
#
#set(TEST_ODB_HEADERS
#        message.hxx)
#
#set(TEST_DEFS)
#set(TEST_DBS)
#
#if(ODB_MYSQL_FOUND)
#    list(APPEND TEST_DEFS DATABASE_MYSQL)
#    list(APPEND TEST_DBS mysql)
#endif()
#
#odb_compile(TEST_SOURCES
#        FILES ${TEST_ODB_HEADERS}
#        MULTI_DATABASE dynamic
#        DB ${TEST_DBS}
#        GENERATE_QUERY
#        GENERATE_SCHEMA
#        SCHEMA_FORMAT sql
#        STANDARD c++11)
#
#add_library(libdbplugin_mysql SHARED ${TEST_SOURCES})
#
#target_link_libraries(libdbplugin_mysql
#        odb odb-mysql)
#
#target_include_directories(libdbplugin_mysql SYSTEM
#        PRIVATE
#        ${ODB_INCLUDE_DIRS}
#        ${ODB_COMPILE_OUTPUT_DIR})
#
#target_compile_options(libdbplugin_mysql
#        PRIVATE
#        -Wall -Wextra -pedantic
#        -fno-strict-aliasing -std=gnu++11 -fPIC)
#
#target_compile_definitions(libdbplugin_mysql
#        PRIVATE ${TEST_DEFS})
#
#set_target_properties(libdbplugin_mysql PROPERTIES
#        PREFIX "")
#
#
#add_executable(plugins_test plugins_test.cpp odb_gen/message_odb.cpp)
#target_link_libraries(plugins_test odb dl)
#
#target_include_directories(plugins_test SYSTEM
#        PRIVATE
#        ${ODB_INCLUDE_DIRS}
#        ${ODB_COMPILE_OUTPUT_DIR})

add_executable(requestTest requestTest.cpp)
target_link_libraries(requestTest ZmqIpc libMsgPack)