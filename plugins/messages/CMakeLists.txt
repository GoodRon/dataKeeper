cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/")
find_package(ODB REQUIRED OPTIONAL_COMPONENTS sqlite pgsql mysql)
include(${ODB_USE_FILE})

foreach(database ${DB_PLUGIN_LIST} )
    set(DATABASE_DEFS)

    if (${database} STREQUAL sqlite)
        set(DATABASE_DEFS DATABASE_SQLITE)
    elseif(${database} STREQUAL mysql)
        set(DATABASE_DEFS DATABASE_MYSQL)
    elseif(${database} STREQUAL pgsql)
        set(DATABASE_DEFS DATABASE_PGSQL)
    endif()

    set(ODB_HEADERS message.hxx)
    set(PLUGIN_SOURCES MessagesConnection.cpp MessagesPlugin.cpp)

    set(DB_NAME messages)

    odb_compile(PLUGIN_SOURCES
            FILES ${ODB_HEADERS}
            MULTI_DATABASE dynamic
            DB ${database}
            DEFS ${DATABASE_DEFS}
            GENERATE_QUERY
            GENERATE_SCHEMA
            OMIT_DROP
            SCHEMA_NAME ${DB_NAME}
#            SCHEMA_FORMAT sql)
            SCHEMA_FORMAT embedded)

    set(PLUGIN_NAME ${DB_NAME}db_${database})

    add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCES})
    target_link_libraries(${PLUGIN_NAME} odb odb-${database} jsonConfigHelper)
    target_include_directories(${PLUGIN_NAME} SYSTEM
            PRIVATE
            ${ODB_INCLUDE_DIRS}
            ${ODB_COMPILE_OUTPUT_DIR})
    target_compile_options(${PLUGIN_NAME}
            PRIVATE
            -Wall -Wextra -pedantic
            -fno-strict-aliasing -std=c++0x -fPIC
            -Wno-unknown-pragmas)
    target_compile_definitions(${PLUGIN_NAME}
            PUBLIC ${DATABASE_DEFS})
    install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION ${LIB_DIR})
endforeach(database)

add_library(messageRequests STATIC MessagesRequests.cpp)
target_link_libraries(messageRequests libMsgPack)
