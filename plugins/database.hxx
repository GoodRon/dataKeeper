/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef DATAKEEPER_DATABASE_HXX
#define DATAKEEPER_DATABASE_HXX

#include <string>
#include <vector>

#if defined(DATABASE_MYSQL)
#  include <odb/mysql/database.hxx>
#elif defined(DATABASE_SQLITE)
#  include <odb/connection.hxx>
#  include <odb/transaction.hxx>
#  include <odb/schema-catalog.hxx>
#  include <odb/sqlite/database.hxx>
#elif defined(DATABASE_PGSQL)
#  include <odb/pgsql/database.hxx>
#elif defined(DATABASE_ORACLE)
#  include <odb/oracle/database.hxx>
#elif defined(DATABASE_MSSQL)
#  include <odb/mssql/database.hxx>
#else
#  error unknown database; did you forget to define the DATABASE_* macros?
#endif

#include <iostream>

/**
 * @brief Функция для инициализации объекта для работы с базой данных
 *
 * @param cmdline параметры командной строки
 * @return database*
 */
inline odb::database* create_database (const std::vector<std::string> cmdline) {
    using namespace std;
    using namespace odb::core;

    database* db = nullptr;

    char **argv = (char**)alloca((cmdline.size() + 1)*(sizeof(char*)));
    int argc = 0;
    for (; static_cast<unsigned>(argc) < cmdline.size(); ++argc) {
        argv[argc] = const_cast<char*>(cmdline[argc].c_str());
    }
    argv[argc] = nullptr;

#if defined(DATABASE_MYSQL)
    db = new odb::mysql::database (argc, argv);
#elif defined(DATABASE_SQLITE)
    db =
    new odb::sqlite::database (
      argc, argv, false, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

  // Create the database schema. Due to bugs in SQLite foreign key
  // support for DDL statements, we need to temporarily disable
  // foreign keys.
  //
  {
    connection_ptr c (db->connection ());

    c->execute ("PRAGMA foreign_keys=OFF");

    transaction t (c->begin ());
    schema_catalog::create_schema (*db);
    t.commit ();

    c->execute ("PRAGMA foreign_keys=ON");
  }
#elif defined(DATABASE_PGSQL)
    db = new odb::pgsql::database (argc, argv);
#elif defined(DATABASE_ORACLE)
    db = new odb::oracle::database (argc, argv);
#elif defined(DATABASE_MSSQL)
    db = new odb::mssql::database (argc, argv);
#endif

    return db;
}

#endif // DATAKEEPER_DATABASE_HXX
