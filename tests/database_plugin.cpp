/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <memory>
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx"
#include "message.hxx"
#include "message_odb.h"

using namespace std;
using namespace odb::core;

extern "C" {

database *returnDatabase(int argc, char **argv) {
    return create_database(argc, argv);
}

}
