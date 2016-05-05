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

namespace journal {

MsgPack::package insertRecord(const std::string &returnAddress, int32_t sa,
                              int32_t da, time_t toper, const std::string &oper) {
    MsgPackVariantMap msg;
    msg[mppPacketType] = packetType;
    msg[mppSource] = returnAddress;
    msg[mppDestination] = destination;
    msg[mppID] = 1;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "insertRecord";
    request["sa"] = sa;
    request["da"] = da;
    request["toper"] = toper;
    request["oper"] = oper;

    msg[mppAdditionalSection] = request;
    return msg.getPackage();
}

MsgPack::package deleteAll(const std::string &returnAddress) {
    MsgPackVariantMap msg;
    msg[mppPacketType] = packetType;
    msg[mppSource] = returnAddress;
    msg[mppDestination] = destination;
    msg[mppID] = 1;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "deleteAll";

    msg[mppAdditionalSection] = request;
    return msg.getPackage();
}

}
