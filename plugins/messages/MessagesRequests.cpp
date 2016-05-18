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

namespace messages_db {

MsgPack::package insertMessage(const std::string &returnAddress, const std::string &source,
                               int64_t sa, int64_t da, int32_t type, int64_t create_time,
                               int64_t io_time, bool exec_status, int32_t status,
                               const std::string &channel, const rawData &data) {
    MsgPackVariantMap msg;
    msg[msPacketType] = packetType;
    msg[msID] = 1;

    MsgPackVariantMap address;
    address[ssSource] = returnAddress;
    address[ssDestination] = destination;

    msg[asServiceSection] = address;

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

    msg[asUserSection] = request;
    return msg.getPackage();
}

MsgPack::package selectMessageByMid(const std::string &returnAddress, int32_t mid) {
    MsgPackVariantMap msg;
    msg[msPacketType] = packetType;
    msg[msID] = 1;

    MsgPackVariantMap address;
    address[ssSource] = returnAddress;
    address[ssDestination] = destination;

    msg[asServiceSection] = address;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "selectMessageByMid";
    request["mid"] = mid;

    msg[asUserSection] = request;
    return msg.getPackage();
}

MsgPack::package selectMessagesByParameters(const std::string &returnAddress,
                                            const std::string &source, int64_t sa,
                                            int64_t da, int32_t type, int32_t status,
                                            const std::string &channel) {
    MsgPackVariantMap msg;
    msg[msPacketType] = packetType;
    msg[msID] = 1;

    MsgPackVariantMap address;
    address[ssSource] = returnAddress;
    address[ssDestination] = destination;

    msg[asServiceSection] = address;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "selectMessagesByParameters";
    request["source"] = source;
    request["sa"] = sa;
    request["da"] = da;
    request["type"] = type;
    request["status"] = status;
    request["channel"] = channel;

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

MsgPack::package deleteMessage(const std::string &returnAddress, int32_t mid) {
    MsgPackVariantMap msg;
    msg[msPacketType] = packetType;
    msg[msID] = 1;

    MsgPackVariantMap address;
    address[ssSource] = returnAddress;
    address[ssDestination] = destination;

    msg[asServiceSection] = address;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "deleteMessage";
    request["mid"] = mid;

    msg[asUserSection] = request;
    return msg.getPackage();
}

MsgPack::package deleteOldMessages(const std::string &returnAddress, unsigned amount,
                                   const std::string &source, int64_t sa, int64_t da,
                                   int32_t type, int32_t status,
                                   const std::string &channel) {
    MsgPackVariantMap msg;
    msg[msPacketType] = packetType;
    msg[msID] = 1;

    MsgPackVariantMap address;
    address[ssSource] = returnAddress;
    address[ssDestination] = destination;

    msg[asServiceSection] = address;

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

    msg[asUserSection] = request;
    return msg.getPackage();
}

MsgPack::package updateStatus(const std::string &returnAddress, int32_t mid,
                              int32_t status) {
    MsgPackVariantMap msg;
    msg[msPacketType] = packetType;
    msg[msID] = 1;

    MsgPackVariantMap address;
    address[ssSource] = returnAddress;
    address[ssDestination] = destination;

    msg[asServiceSection] = address;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "updateStatus";
    request["mid"] = mid;
    request["status"] = status;

    msg[asUserSection] = request;
    return msg.getPackage();
}

MsgPack::package updateChannel(const std::string &returnAddress, int32_t mid,
                               const std::string &channel) {
    MsgPackVariantMap msg;
    msg[msPacketType] = packetType;
    msg[msID] = 1;

    MsgPackVariantMap address;
    address[ssSource] = returnAddress;
    address[ssDestination] = destination;

    msg[asServiceSection] = address;

    MsgPackVariantMap request;
    request["database"] = database;
    request["request"] = "updateChannel";
    request["mid"] = mid;
    request["channel"] = channel;

    msg[asUserSection] = request;
    return msg.getPackage();
}

}

