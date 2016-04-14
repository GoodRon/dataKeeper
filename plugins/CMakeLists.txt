cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/")
find_package(ODB REQUIRED OPTIONAL_COMPONENTS sqlite pgsql mysql)
include(${ODB_USE_FILE})

#add_library(jsonConfigHelper SHARED jsonConfigHelper.cpp)
#target_link_libraries(jsonConfigHelper jsoncpp)

foreach(database pgsql sqlite mysql)
    set(DATABASE_DEFS)

    if (${database} STREQUAL sqlite)
        set(DATABASE_DEFS DATABASE_SQLITE)
    elseif(${database} STREQUAL mysql)
        set(DATABASE_DEFS DATABASE_MYSQL)
    elseif(${database} STREQUAL pgsql)
        set(DATABASE_DEFS DATABASE_PGSQL)
    endif()

    set(ODB_HEADERS message.hxx)
    set(PLUGIN_SOURCES MessagesConnection.cpp MessagesPlugin.cpp jsonConfigHelper.cpp)

    odb_compile(PLUGIN_SOURCES
            FILES ${ODB_HEADERS}
            MULTI_DATABASE dynamic
            DB ${database}
            GENERATE_QUERY
            GENERATE_SCHEMA
            SCHEMA_FORMAT embedded
            STANDARD c++11)

    set(PLUGIN_NAME messagesdb_${database})

    add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCES})
    target_link_libraries(${PLUGIN_NAME} odb odb-${database} jsoncpp)
    target_include_directories(${PLUGIN_NAME} SYSTEM
            PRIVATE
            ${ODB_INCLUDE_DIRS}
            ${ODB_COMPILE_OUTPUT_DIR})
    target_compile_options(${PLUGIN_NAME}
            PRIVATE
            -Wall -Wextra -pedantic
            -fno-strict-aliasing -std=gnu++11 -fPIC
            -Wno-unknown-pragmas)
    target_compile_definitions(${PLUGIN_NAME}
            PRIVATE ${DATABASE_DEFS})
endforeach(database)
