/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <chrono>

#include "ipc_const.h"
#include "ipc_fdnotify_recv.h"
#include "ipc_msgpack.h"
#include "MsgPack_pack.h"
#include "JournalRequests.hxx"

using namespace std;
using namespace MsgPack;
using namespace ipc;
using namespace chrono;
using namespace journal_db;

int main() {
    fdnotify_recv ipc(SOCK_DEFAULT, "insertRecordTest");

    auto pckg = insertRecord(123, 456, system_clock::to_time_t(system_clock::now()), "test operation",
                             "insertRecordTest");
    busipc_client::RawData data(pckg.begin(), pckg.end());
    ipc.SendRep(IpcCmd_Msgpack, 1, "dataKeeper", data);
    return 0;
}
