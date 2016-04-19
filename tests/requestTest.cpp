/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <iostream>
#include <string>
#include <chrono>

#include "ipc_const.h"
#include "ipc_fdnotify_recv.h"
#include "ipc_msgpack.h"
#include "MsgPackVariantMap.h"
#include "MsgPackVariant.h"
#include "MsgPackProto.hxx"

using namespace std;
using namespace chrono;
using namespace MsgPack;
using namespace ipc;

int main() {
    fdnotify_recv ipc(SOCK_DEFAULT, "requestTest");

    MsgPackVariantMap msg;
    msg[mppPacketType] = "Database";
    msg[mppSource] = "requestTest";
    msg[mppDestination] = "dataKeeper";
    msg[mppID] = 1;

    MsgPackVariantMap request;
    request["database"] = "messages";
    request["request"] = "insertMessage";
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
    request["data"] = "the long string for the testing purposes with no meaning";

    msg[mppAdditionalSection] = request;

    auto package = msg.getPackage();
    busipc_client::RawData data(package.begin(), package.end());

    cout << ipc.SendRep(IpcCmd_Msgpack, 1, "dataKeeper", data) << endl;
    return 0;
}
