/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <syslog.h>

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
    syslog(LOG_INFO, "Message has been translated to MessagesPlugin");

    if (!m_database) {
        syslog(LOG_ERR, "There is no instance for database");
        return false;
    }

    if (!request.contain("request")) {
        syslog(LOG_ERR, "There is no request in package");
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

    if (request["request"].toString() == "updateStatus") {
        return updateStatus(request, answer);
    }

    if (request["request"].toString() == "updateChannel") {
        return updateChannel(request, answer);
    }

    return false;
}

void MessagesConnection::instantiateDatabase() {
    vector<string> args;
    if (!jsonToCmdLine(m_jsonConf, args)) {
        syslog(LOG_ERR, "Can't cast jsonConf to cmdline");
        return;
    }

    auto db = create_database(args);
    create_embedded_schema(db);
    syslog(LOG_INFO, "Database was instantiated");
    m_database.reset(db);
}

bool MessagesConnection::insertMessage(const MsgPack::MsgPackVariantMap& request,
                                       MsgPack::MsgPackVariantMap& answer) {
    Message message(request["source"].toString(), request["sa"].toInt64(),
                    request["da"].toInt64(), request["type"].toInt32(),
                    request["create_time"].toInt64(), request["io_time"].toInt64(),
                    request["exec_status"].toBool(), request["status"].toInt32(),
                    request["channel"].toString(), request["data"].toBin());

    transaction t(m_database->begin());
    try {
        auto mid = m_database->persist(message);
        answer["mid"] = static_cast<int64_t>(mid);
    } catch (odb::exception& ex) {
        t.rollback();
        syslog(LOG_ERR, "Exception: %s", ex.what());
        return false;
    }
    t.commit();
    return true;
}

bool MessagesConnection::selectMessageByMid(const MsgPack::MsgPackVariantMap &request,
                                            MsgPack::MsgPackVariantMap &answer) {
    typedef odb::query<Message> query;
    typedef odb::result<Message> result;

    transaction t (m_database->begin());
    try {
        result r (m_database->query<Message>(query::mid == request["mid"].toInt64()));

        result::iterator i(r.begin());
        if (i != r.end()) {
            auto data = i->getData();
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
    } catch (odb::exception& ex) {
        t.rollback();
        syslog(LOG_ERR, "Exception: %s", ex.what());
        return false;
    }
    t.commit();
    return true;
}

bool MessagesConnection::deleteAll(const MsgPack::MsgPackVariantMap&,
                                   MsgPack::MsgPackVariantMap&) {
    transaction t(m_database->begin());
    try {
        m_database->erase_query<Message>();
    } catch (odb::exception& ex) {
        t.rollback();
        syslog(LOG_ERR, "Exception: %s", ex.what());
        return false;
    }
    t.commit();
    return true;
}

bool MessagesConnection::deleteMessage(const MsgPack::MsgPackVariantMap& request,
                                       MsgPack::MsgPackVariantMap&) {
    transaction t (m_database->begin());
    try {
        m_database->erase<Message>(request["mid"].toInt64());
    } catch (odb::exception& ex) {
        t.rollback();
        syslog(LOG_ERR, "Exception: %s", ex.what());
        return false;
    }
    t.commit();
    return true;
}

bool MessagesConnection::deleteOldMessages(const MsgPack::MsgPackVariantMap& request,
                                           MsgPack::MsgPackVariantMap&) {
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

    transaction t (m_database->begin());
    try {
        result r (m_database->query<Message>(q));

        vector<int64_t> mids;

        for (auto i = r.begin(); i != r.end(); ++i) {
            mids.push_back(i->getMid());
        }

        long counter = mids.size() - request["amount"].toUInt32();
        if (counter > 0) {
            for (int i = 0; i < counter; ++i) {
                m_database->erase<Message>(mids[i]);
            }
        }
    } catch (odb::exception& ex) {
        t.rollback();
        syslog(LOG_ERR, "Exception: %s", ex.what());
        return false;
    }
    t.commit();
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

    MsgPackVariantArray mids;
    transaction t(m_database->begin());
    try {
        result r(m_database->query<Message>(q));

        for (auto i = r.begin(); i != r.end(); ++i) {
            mids.push_back(static_cast<int64_t>(i->getMid()));
        }
    } catch (odb::exception& ex) {
        t.rollback();
        syslog(LOG_ERR, "Exception: %s", ex.what());
        return false;
    }
    t.commit();
    answer["mids"] = mids;
    return true;
}

bool MessagesConnection::updateStatus(const MsgPack::MsgPackVariantMap& request,
                                      MsgPack::MsgPackVariantMap&) {
    transaction t(m_database->begin ());
    try {
        shared_ptr<Message> msg(m_database->load<Message>(request["mid"].toInt64()));
        msg->setStatus(request["status"].toInt32());
        m_database->update(*msg);
    } catch (odb::exception& ex) {
        t.rollback();
        syslog(LOG_ERR, "Exception: %s", ex.what());
        return false;
    }
    t.commit();
    return true;
}

bool MessagesConnection::updateChannel(const MsgPack::MsgPackVariantMap& request,
                                       MsgPack::MsgPackVariantMap&) {
    transaction t(m_database->begin());
    try {
        shared_ptr<Message> msg (m_database->load<Message>(request["mid"].toInt64()));
        msg->setChannel(request["channel"].toString());
        m_database->update(*msg);
    } catch (odb::exception& ex) {
        t.rollback();
        syslog(LOG_ERR, "Exception: %s", ex.what());
        return false;
    }
    t.commit();
    return true;
}
