/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx"
#include "message.hxx"
#include "message_odb.h"

#include "MessagesConnection.hxx"
#include "MsgPackVariantMap.h"
#include "MsgPackVariant.h"
#include "jsonConfigHelper.hxx"

using namespace std;
using namespace MsgPack;
using namespace odb;

MessagesConnection::MessagesConnection(const std::string &jsonConf):
    AbstractConnection(jsonConf) {

    instantiateDatabase();
}

MessagesConnection::~MessagesConnection() {

}

bool MessagesConnection::processQuery(const MsgPackVariantMap &request,
                                      MsgPackVariantMap &answer) {
    cout << "Message has been translated to MessagesPlugin" << endl;

    if (!m_database) {
        cout << "No instance for database" << endl;
        return false;
    }

    if (!request.contain("request")) {
        cout << "No request" << endl;
        return false;
    }

    if (request["request"].toString() == "insert") {
        return insertMessage(request);
    }



    return false;
}

void MessagesConnection::instantiateDatabase() {
    std::vector<std::string> args;
    if (!jsonToCmdLine(m_jsonConf, args)) {
        cout << "Can't cast jsonConf to cmdline" << endl;
        return;
    }

    auto db = create_database(args);
    cout << "database was instantiated" << endl;
    m_database.reset(db);
}

bool MessagesConnection::insertMessage(const MsgPack::MsgPackVariantMap& request) {
    cout << "Message has been proceeded" << endl;

    Message message(request["source"].toString(), request["sa"].toInt64(),
                    request["da"].toInt64(), request["type"].toInt32(),
                    request["create_time"].toInt64(), request["io_time"].toInt64(),
                    request["exec_status"].toBool(), request["status"].toInt32(),
                    request["channel"].toString(), request["data"].toBin());

    transaction t(m_database->begin());
    m_database->persist(message);
    t.commit();

    typedef odb::query<Message> query;
    typedef odb::result<Message> result;


    // TEST
    transaction t2(m_database->begin ());

    result r(m_database->query<Message> (query::mid > 0));

    for (result::iterator i (r.begin ()); i != r.end (); ++i) {
        cout << "Hello, " << i->getMid () << " " << i->getStatus() << "!" << endl;
    }

    t2.commit ();
    return true;
}
