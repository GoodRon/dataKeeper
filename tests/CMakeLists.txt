cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/")

find_package(ODB REQUIRED OPTIONAL_COMPONENTS sqlite pgsql mysql)

include(${ODB_USE_FILE})

if(ODB_SQLITE_FOUND AND NOT ODB_PGSQL_FOUND AND NOT ODB_MYSQL_FOUND)
    message(FATAL_ERROR "Neither mysql nor sqlite and nor pqsql odb module was found")
endif()

set(TEST_SOURCES
        db_test.cpp)

set(TEST_ODB_HEADERS
        message.hxx)

set(TEST_DEFS)
set(TEST_DBS)

if(ODB_PGSQL_FOUND)
    list(APPEND TEST_DEFS DATABASE_PGSQL)
    list(APPEND TEST_DBS pgsql)
endif()

#if(ODB_MYSQL_FOUND)
#    list(APPEND TEST_DEFS DATABASE_MYSQL)
#    list(APPEND TEST_DBS mysql)
#endif()

if(ODB_SQLITE_FOUND)
    list(APPEND TEST_DEFS DATABASE_SQLITE)
    list(APPEND TEST_DBS sqlite)
endif()

odb_compile(TEST_SOURCES
        FILES ${TEST_ODB_HEADERS}
        MULTI_DATABASE dynamic
        DB ${TEST_DBS}
        GENERATE_QUERY
        GENERATE_SCHEMA
        SCHEMA_FORMAT embedded
        STANDARD c++11)

add_executable(db_test ${TEST_SOURCES})

target_link_libraries(db_test
        ${ODB_LIBRARIES})

target_include_directories(db_test SYSTEM
        PRIVATE
        ${ODB_INCLUDE_DIRS}
        ${ODB_COMPILE_OUTPUT_DIR})

target_compile_options(db_test
        PRIVATE
        -Wall -Wextra -pedantic
        -fno-strict-aliasing -std=gnu++11)

target_compile_definitions(db_test
        PRIVATE ${TEST_DEFS})

set_target_properties(db_test PROPERTIES
        PREFIX "")