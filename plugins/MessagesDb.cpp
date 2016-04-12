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

using namespace MsgPack;

MessagesDb::MessagesDb(const std::string &jsonConf):
    AbstractConnection(jsonConf) {


    auto db = create_database();
    m_database.reset(db);

}

MessagesDb::~MessagesDb() {

}

bool MessagesDb::processQuery(const MsgPackVariantMap &request,
                              MsgPackVariantMap &answer) {

}