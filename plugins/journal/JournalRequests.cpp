/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include "JournalRequests.hxx"
#include "MsgPackVariantMap.h"
#include "MsgPackVariant.h"
#include "MsgPackProto.hxx"

using namespace std;
using namespace MsgPack;

const string packetType = "Database";
const string destination = "dataKeeper";
const string database = "journal";

namespace journal_db {

MsgPack::package insertRecord(const std::string &returnAddress, int32_t sa,
                              int32_t da, time_t toper, const std::string &oper) {
    MsgPackVariantMap msg;
    msg[msPacketType] = packetType;
    msg[msID] = 1;

    MsgPackVariantMap address;
    address[ssSource] = returnAddress;
    address[ssDestination] = destination;

    msg[asServiceSection] = address;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "insertRecord";
    request["sa"] = sa;
    request["da"] = da;
    request["toper"] = static_cast<int>(toper);
    request["oper"] = oper;

    msg[asUserSection] = request;
    return msg.getPackage();
}

MsgPack::package deleteAll(const std::string &returnAddress) {
    MsgPackVariantMap msg;
    msg[msPacketType] = packetType;
    msg[msID] = 1;

    MsgPackVariantMap address;
    address[ssSource] = returnAddress;
    address[ssDestination] = destination;

    msg[asServiceSection] = address;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "deleteAll";

    msg[asUserSection] = request;
    return msg.getPackage();
}

MsgPack::package deleteOldRecords(const std::string &returnAddress, unsigned amount) {
    MsgPackVariantMap msg;
    msg[msPacketType] = packetType;
    msg[msID] = 1;

    MsgPackVariantMap address;
    address[ssSource] = returnAddress;
    address[ssDestination] = destination;

    msg[asServiceSection] = address;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "deleteOldRecords";
    request["amount"] = amount;

    msg[asUserSection] = request;
    return msg.getPackage();
}

MsgPack::package selectRecordsByParameters(const std::string &returnAddress,
                                           int32_t sa, int32_t da,
                                           time_t topermax, time_t topermin) {
    MsgPackVariantMap msg;
    msg[msPacketType] = packetType;
    msg[msID] = 1;

    MsgPackVariantMap address;
    address[ssSource] = returnAddress;
    address[ssDestination] = destination;

    msg[asServiceSection] = address;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "selectRecordsByParameters";
    request["sa"] = sa;
    request["da"] = da;
    request["topermin"] = static_cast<int>(topermin);
    request["topermax"] = static_cast<int>(topermax);

    msg[asUserSection] = request;
    return msg.getPackage();
}

MsgPack::package selectRecordByMid(const std::string &returnAddress, int64_t mid) {
    MsgPackVariantMap msg;
    msg[msPacketType] = packetType;
    msg[msID] = 1;

    MsgPackVariantMap address;
    address[ssSource] = returnAddress;
    address[ssDestination] = destination;

    msg[asServiceSection] = address;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "selectRecordByMid";
    request["mid"] = mid;

    msg[asUserSection] = request;
    return msg.getPackage();
}

}
