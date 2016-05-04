/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include "ipc_const.h"
#include "ipc_fdnotify_recv.h"
#include "MessagesRequests.hxx"
#include "ipc_msgpack.h"

using namespace ipc;

int main() {
    fdnotify_recv ipc(SOCK_DEFAULT, "deleteAllTest");

    auto pckg = deleteOldMessages("deleteAllTest", "", -1, -1, -1, 0, "", 5);
    busipc_client::RawData data(pckg.begin(), pckg.end());
    ipc.SendRep(IpcCmd_Msgpack, 1, "dataKeeper", data);
    return 0;
}