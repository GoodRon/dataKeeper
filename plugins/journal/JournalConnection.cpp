/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <syslog.h>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx"
#include "journal.hxx"
#include "journal_odb.h"
#include "JournalConnection.hxx"
#include "MsgPackVariantMap.h"
#include "MsgPackVariant.h"
#include "jsonConfigHelper.hxx"

using namespace std;
using namespace MsgPack;
using namespace odb;

JournalConnection::JournalConnection(const std::string &jsonConf):
        AbstractConnection(jsonConf) {
    instantiateDatabase();
}

JournalConnection::~JournalConnection() {
}

bool JournalConnection::processQuery(const MsgPackVariantMap& request,
                                     MsgPackVariantMap& answer) {
    syslog(LOG_INFO, "Message has been translated to JournalPlugin");

    if (!m_database) {
        syslog(LOG_ERR, "There is no instance for database");
        return false;
    }

    if (!request.contain("request")) {
        syslog(LOG_ERR, "There is no request in package");
        return false;
    }

    if (request["request"].toString() == "insertRecord") {
        return insertRecord(request, answer);
    }

    if (request["request"].toString() == "deleteAll") {
        return deleteAll(request, answer);
    }

    return false;
}

void JournalConnection::instantiateDatabase() {
    vector<string> args;
    if (!jsonToCmdLine(m_jsonConf, args)) {
        syslog(LOG_ERR, "Can't cast jsonConf to cmdline");
        return;
    }

    auto db = create_database(args);
    syslog(LOG_INFO, "Database was instantiated");
    m_database.reset(db);
}

bool JournalConnection::insertRecord(const MsgPack::MsgPackVariantMap& request,
                                     MsgPack::MsgPackVariantMap& answer) {
    Journal record(request["sa"].toInt32(), request["da"].toInt32(),
                   request["toper"].toInt64(), request["oper"].toString());

    transaction t(m_database->begin());
    try {
        auto mid = m_database->persist(record);
        answer["mid"] = static_cast<int64_t>(mid);
    } catch (odb::exception& ex) {
        t.rollback();
        syslog(LOG_ERR, "Exception: %s", ex.what());
        return false;
    }
    t.commit();
    return true;
}

bool JournalConnection::deleteAll(const MsgPack::MsgPackVariantMap& request,
                                  MsgPack::MsgPackVariantMap& answer) {
    transaction t(m_database->begin());
    try {
        m_database->erase_query<Journal>();
    } catch (odb::exception& ex) {
        t.rollback();
        syslog(LOG_ERR, "Exception: %s", ex.what());
        return false;
    }
    t.commit ();
    return true;
}
