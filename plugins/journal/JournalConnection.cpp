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
#include "MsgPackVariantArray.h"
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

    if (request["request"].toString() == "deleteOldRecords") {
        return deleteOldRecords(request, answer);
    }

    if (request["request"].toString() == "selectRecordsByParameters") {
        return selectRecordsByParameters(request, answer);
    }

    if (request["request"].toString() == "selectRecordByMid") {
        return selectRecordByMid(request, answer);
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
    create_embedded_schema(db, "journal");
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

bool JournalConnection::deleteAll(const MsgPack::MsgPackVariantMap&,
                                  MsgPack::MsgPackVariantMap&) {
    transaction t(m_database->begin());
    try {
        m_database->erase_query<Journal>();
    } catch (odb::exception& ex) {
        t.rollback();
        syslog(LOG_ERR, "Exception: %s", ex.what());
        return false;
    }
    t.commit();
    return true;
}

bool JournalConnection::deleteOldRecords(const MsgPack::MsgPackVariantMap& request,
                                         MsgPack::MsgPackVariantMap&) {
    typedef odb::result<Journal> result;

    transaction t(m_database->begin());
    try {
        result r(m_database->query<Journal>());

        vector<int64_t> mids;
        for (auto i = r.begin(); i != r.end(); ++i) {
            mids.push_back(i->getMid());
        }

        long counter = mids.size() - request["amount"].toUInt32();
        if (counter > 0) {
            for (int i = 0; i < counter; ++i) {
                m_database->erase<Journal>(mids[i]);
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

bool JournalConnection::selectRecordsByParameters(const MsgPack::MsgPackVariantMap& request,
                                                  MsgPack::MsgPackVariantMap& answer) {
    typedef odb::query<Journal> query;
    typedef odb::result<Journal> result;

    query q;

    if (request["sa"].toInt32() != -1) {
        q = q && query(query::sa == request["sa"].toInt32());
    }

    if (request["da"].toInt32() != -1) {
        q = q && query(query::da == request["da"].toInt32());
    }

    if (request["topermin"].toInt64() != 0) {
        q = q && query(query::toper >= request["topermin"].toInt64());
    }

    if (request["topermax"].toInt64() != 0) {
        q = q && query(query::toper <= request["topermax"].toInt64());
    }

    MsgPackVariantArray mids;
    transaction t(m_database->begin());
    try {
        result r(m_database->query<Journal>(q));

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

bool JournalConnection::selectRecordByMid(const MsgPack::MsgPackVariantMap& request,
                                          MsgPack::MsgPackVariantMap& answer) {
    typedef odb::query<Journal> query;
    typedef odb::result<Journal> result;

    transaction t(m_database->begin());
    try {
        result r(m_database->query<Journal>(query::mid == request["mid"].toInt64()));

        result::iterator i(r.begin());
        if (i != r.end()) {
            answer["mid"] = static_cast<int64_t>(i->getMid());
            answer["sa"] = static_cast<int32_t>(i->getSA());
            answer["da"] = static_cast<int32_t>(i->getDA());
            answer["toper"] = static_cast<int64_t>(i->getToper());
            answer["oper"] = i->getOper();
        }
    } catch (odb::exception& ex) {
        t.rollback();
        syslog(LOG_ERR, "Exception: %s", ex.what());
        return false;
    }
    t.commit();
    return true;
}
