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
#include "MsgPackVariantArray.h"

using namespace std;
using namespace MsgPack;
using namespace odb;

MessagesConnection::MessagesConnection(const std::string &jsonConf):
    AbstractConnection(jsonConf) {

    instantiateDatabase();
}

MessagesConnection::~MessagesConnection() {

}

bool MessagesConnection::processQuery(const MsgPackVariantMap& request,
                                      MsgPackVariantMap& answer) {
    cout << "Message has been translated to MessagesPlugin" << endl;

    if (!m_database) {
        cout << "No instance for database" << endl;
        return false;
    }

    if (!request.contain("request")) {
        cout << "No request" << endl;
        return false;
    }

    if (request["request"].toString() == "insertMessage") {
        return insertMessage(request, answer);
    }

    if (request["request"].toString() == "selectMessageByMid") {
        return selectMessageByMid(request, answer);
    }

    if (request["request"].toString() == "deleteAll") {
        return deleteAll(request, answer);
    }

    if (request["request"].toString() == "deleteMessage") {
        return deleteMessage(request, answer);
    }

    if (request["request"].toString() == "deleteOldMessages") {
        return deleteOldMessages(request, answer);
    }

    if (request["request"].toString() == "selectMessagesByParameters") {
        return selectMessagesByParameters(request, answer);
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

bool MessagesConnection::insertMessage(const MsgPack::MsgPackVariantMap& request,
                                       MsgPack::MsgPackVariantMap& answer) {
    auto str = request["data"].toString();
    cout << "message data: " << str << endl;

    Message message(request["source"].toString(), request["sa"].toInt64(),
                    request["da"].toInt64(), request["type"].toInt32(),
                    request["create_time"].toInt64(), request["io_time"].toInt64(),
                    request["exec_status"].toBool(), request["status"].toInt32(),
                    request["channel"].toString(), request["data"].toBin());

    transaction t(m_database->begin());
    auto mid = m_database->persist(message);
    t.commit();

    answer["mid"] = static_cast<int64_t>(mid);
    return true;
}

bool MessagesConnection::selectMessageByMid(const MsgPack::MsgPackVariantMap &request,
                                            MsgPack::MsgPackVariantMap &answer) {
    typedef odb::query<Message> query;
    typedef odb::result<Message> result;

    transaction t (m_database->begin ());
    result r (m_database->query<Message> (query::mid == request["mid"].toInt64()));

    cout << "requested mid = " << request["mid"].toInt64() << endl;

    result::iterator i (r.begin ());
    if (i != r.end()) {
        auto data = i->getData();
        string str(data.begin(), data.end());
        cout << "data: " << str << endl;

        cout << "something was found" << endl;

        answer["mid"] = static_cast<int64_t>(i->getMid());
        answer["source"] = i->getSource();
        answer["sa"] = static_cast<int64_t>(i->getSA());
        answer["da"] = static_cast<int64_t>(i->getDA());
        answer["type"] = i->getType();
        answer["create_time"] = static_cast<int64_t>(i->getCreateTime());
        answer["io_time"] = static_cast<int64_t>(i->getIoTime());
        answer["exec_status"] = i->getExecStatus();
        answer["status"] = i->getStatus();
        answer["channel"] = i->getChannel();
        answer["data"] = i->getData();
    }
    t.commit ();
    return true;
}

bool MessagesConnection::deleteAll(const MsgPack::MsgPackVariantMap&,
                                   MsgPack::MsgPackVariantMap&) {
    transaction t (m_database->begin ());
    m_database->erase_query<Message>();
    t.commit ();
    return true;
}

bool MessagesConnection::deleteMessage(const MsgPack::MsgPackVariantMap& request,
                                       MsgPack::MsgPackVariantMap&) {
    transaction t (m_database->begin ());
    m_database->erase<Message>(request["mid"].toInt64());
    t.commit ();
    return true;
}

bool MessagesConnection::deleteOldMessages(const MsgPack::MsgPackVariantMap& request,
                                           MsgPack::MsgPackVariantMap& answer) {
    typedef odb::query<Message> query;
    typedef odb::result<Message> result;

    query q;

    if (request["source"].toString() != "") {
        q = q && query(query::source == request["source"].toString());
    }

    if (request["sa"].toInt64() != -1) {
        q = q && query(query::sa == request["sa"].toInt64());
    }

    if (request["da"].toInt64() != -1) {
        q = q && query(query::da == request["da"].toInt64());
    }

    if (request["type"].toInt32() != -1) {
        q = q && query(query::type == request["type"].toInt32());
    }

    if (request["status"].toInt32() != -1) {
        q = q && query(query::status == request["status"].toInt32());
    }

    if (request["channel"].toString() != "") {
        q = q && query(query::channel == request["channel"].toString());
    }

    q = q + query("ORDER BY" + query::create_time);

    transaction t (m_database->begin ());
    result r (m_database->query<Message> (q));

    vector<int64_t> mids;

    for (auto i = r.begin(); i != r.end(); ++i) {
        cout << "mid: " << i->getMid() << endl;
        mids.push_back(i->getMid());
    }

    long counter = mids.size() - request["amount"].toUInt32();
    if (counter > 0) {
        for (int i = 0; i < counter; ++i) {
            m_database->erase<Message>(mids[i]);
        }
    }

    t.commit ();
    return true;
}

bool MessagesConnection::selectMessagesByParameters(const MsgPack::MsgPackVariantMap& request,
                                                    MsgPack::MsgPackVariantMap& answer) {
    typedef odb::query<Message> query;
    typedef odb::result<Message> result;

    query q;

    if (request["source"].toString() != "") {
        q = q && query(query::source == request["source"].toString());
    }

    if (request["sa"].toInt64() != -1) {
        q = q && query(query::sa == request["sa"].toInt64());
    }

    if (request["da"].toInt64() != -1) {
        q = q && query(query::da == request["da"].toInt64());
    }

    if (request["type"].toInt32() != -1) {
        q = q && query(query::type == request["type"].toInt32());
    }

    if (request["status"].toInt32() != -1) {
        q = q && query(query::status == request["status"].toInt32());
    }

    if (request["channel"].toString() != "") {
        q = q && query(query::channel == request["channel"].toString());
    }

    q = q + query("ORDER BY" + query::create_time);

    transaction t (m_database->begin ());
    result r (m_database->query<Message> (q));

    MsgPackVariantArray mids;

    for (auto i = r.begin(); i != r.end(); ++i) {
        cout << "mid: " << i->getMid() << endl;
        mids.push_back(i->getMid());
    }

    answer["mids"] = mids;
    t.commit();
    return true;
}