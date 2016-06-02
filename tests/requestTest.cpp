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
#include "MsgPackProto.hxx"
#include "MsgPack_pack.h"
#include "MessagesRequests.hxx"

using namespace std;
using namespace chrono;
using namespace MsgPack;
using namespace ipc;
using namespace messages_db;

int main() {
    fdnotify_recv ipc(SOCK_DEFAULT, "requestTest");

    string str = "the long string for the testing purposes with no meaning";

    auto pckg = insertMessage("unknown", 512, 1024, 256, system_clock::to_time_t(system_clock::now()),
                              system_clock::to_time_t(system_clock::now()), true, 0, "TestChannel",
                              rawData(str.begin(), str.end()), "requestTest");

    busipc_client::RawData data(pckg.begin(), pckg.end());

    ipc.SendRep(IpcCmd_Msgpack, 1, "dataKeeper", data);

    pckg = insertMessage("someone", 312, 42, 512, system_clock::to_time_t(system_clock::now()),
                         system_clock::to_time_t(system_clock::now()), true, 0, "TestChannel",
                         rawData(str.begin(), str.end()), "requestTest");

    busipc_client::RawData newData(pckg.begin(), pckg.end());

    ipc.SendRep(IpcCmd_Msgpack, 1, "dataKeeper", newData);
    return 0;
}
