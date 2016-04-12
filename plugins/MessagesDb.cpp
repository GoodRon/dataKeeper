/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx"
#include "message.hxx"
#include "message_odb.h"

#include "MessagesDb.hxx"
#include "MsgPackVariantMap.h"
#include "jsonConfigHelper.hxx"

using namespace MsgPack;

MessagesDb::MessagesDb(const std::string &jsonConf):
    AbstractConnection(jsonConf) {

    instantiateDatabase();
}

MessagesDb::~MessagesDb() {

}

bool MessagesDb::processQuery(const MsgPackVariantMap &request,
                              MsgPackVariantMap &answer) {
    if (!m_database) {
        return false;
    }


}

void MessagesDb::instantiateDatabase() {
    std::vector<std::string> args;
    if (!jsonToCmdLine(m_jsonConf, args)) {
        return;
    }

    auto db = create_database(args);
    m_database.reset(db);
}
