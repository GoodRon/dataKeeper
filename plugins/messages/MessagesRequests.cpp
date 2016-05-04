/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include "MessagesRequests.hxx"
#include "MsgPackVariantMap.h"
#include "MsgPackVariant.h"
#include "MsgPackProto.hxx"

using namespace std;
using namespace MsgPack;

const string packetType = "Database";
const string destination = "dataKeeper";
const string database = "messages";

MsgPack::package insertMessage(const std::string& returnAddress, const std::string& source,
                               uint64_t sa, uint64_t da, int32_t type, int64_t create_time,
                               int64_t io_time, bool exec_status, int32_t status,
                               const std::string& channel, const rawData& data) {
    MsgPackVariantMap msg;
    msg[mppPacketType] = packetType;
    msg[mppSource] = returnAddress;
    msg[mppDestination] = destination;
    msg[mppID] = 1;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "insertMessage";
    request["source"] = source;
    request["sa"] = sa;
    request["da"] = da;
    request["type"] = type;
    request["create_time"] = create_time;
    request["io_time"] = io_time;
    request["exec_status"] = exec_status;
    request["status"] = status;
    request["channel"] = channel;
    request["data"] = data;

    msg[mppAdditionalSection] = request;
    return msg.getPackage();
}

MsgPack::package selectMessageByMid(const std::string &returnAddress, int64_t mid) {
    MsgPackVariantMap msg;
    msg[mppPacketType] = packetType;
    msg[mppSource] = returnAddress;
    msg[mppDestination] = destination;
    msg[mppID] = 1;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "selectMessageByMid";
    request["mid"] = mid;

    msg[mppAdditionalSection] = request;
    return msg.getPackage();
}

MsgPack::package deleteAll(const std::string& returnAddress) {
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

MsgPack::package deleteMessage(const std::string& returnAddress, int64_t mid) {
    MsgPackVariantMap msg;
    msg[mppPacketType] = packetType;
    msg[mppSource] = returnAddress;
    msg[mppDestination] = destination;
    msg[mppID] = 1;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "deleteMessage";
    request["mid"] = mid;

    msg[mppAdditionalSection] = request;
    return msg.getPackage();
}

MsgPack::package deleteOldMessages(const std::string& returnAddress, const std::string& source,
                                   int64_t sa, int64_t da, int32_t type, int32_t status,
                                   const std::string& channel, unsigned amount) {
    MsgPackVariantMap msg;
    msg[mppPacketType] = packetType;
    msg[mppSource] = returnAddress;
    msg[mppDestination] = destination;
    msg[mppID] = 1;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "deleteOldMessages";
    request["source"] = source;
    request["sa"] = sa;
    request["da"] = da;
    request["type"] = type;
    request["status"] = status;
    request["channel"] = channel;
    request["amount"] = amount;

    msg[mppAdditionalSection] = request;
    return msg.getPackage();
}
