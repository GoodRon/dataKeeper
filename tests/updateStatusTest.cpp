/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <iostream>

#include "ipc_const.h"
#include "ipc_fdnotify_recv.h"
#include "ipc_msgpack.h"
#include "MsgPackProto.hxx"
#include "MsgPack_pack.h"
#include "MessagesRequests.hxx"

using namespace std;
using namespace MsgPack;
using namespace ipc;
using namespace messages_db;

int main() {
    fdnotify_recv ipc(SOCK_DEFAULT, "updateStatusTest");
    auto pckg = updateStatus(1, 5, "updateStatusTest");
    busipc_client::RawData data(pckg.begin(), pckg.end());
    ipc.SendRep(IpcCmd_Msgpack, 1, "dataKeeper", data);
    return 0;
}