/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <iostream>
#include <string>
#include <chrono>

#include "ipc_fdnotify_recv.h"
#include "ipc_msgpack.h"
#include "MsgPackVariantMap.h"
#include "MsgPackVariant.h"
#include "MsgPackProto.hxx"

using namespace std;
using namespace chrono;
using namespace MsgPack;
using namespace ipc;

const string ipcSock = "/tmp/keeper_ipc";

int main() {
    fdnotify_recv ipc(ipcSock.c_str(), "requestTest");

    MsgPackVariantMap msg;
    msg[mppPacketType] = "Database";
    msg[mppSource] = "requestTest";
    msg[mppDestination] = "dataKeeper";
    msg[mppID] = 1;

    MsgPackVariantMap request;
    request["table"] = "messages";
    request["request"] = "insert";
    request["source"] = "SourceObject";
    request["sa"] = 42;
    request["da"] = 17;
    request["type"] = 256;
    request["exttype"] = "none";
    request["create_time"] = system_clock::to_time_t(system_clock::now());
    request["io_time"] = system_clock::to_time_t(system_clock::now());
    request["exec_status"] = true;
    request["status"] = 0;
    request["channel"] = "testChannel";
    request["data"] = "";

    msg[mppAdditionalSection] = request;

    auto package = msg.getPackage();
    busipc_client::RawData data(package.begin(), package.end());
    ipc.SendRep(IpcCmd_Msgpack, 1, "dataKeeper", data);
    return 0;
}
