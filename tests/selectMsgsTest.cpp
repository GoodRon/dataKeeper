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
#include "MsgPackVariantArray.h"
#include "MsgPackProto.hxx"
#include "MessagesRequests.hxx"

using namespace std;
using namespace MsgPack;
using namespace ipc;
using namespace messages_db;

int main() {
    fdnotify_recv ipc(SOCK_DEFAULT, "selectMsgsTest");

    auto pckg = selectMessagesByParameters("unknown", -1, -1, -1, 0, "", "selectMsgsTest");
    busipc_client::RawData data(pckg.begin(), pckg.end());

    cout << ipc.SendRep(IpcCmd_Msgpack, 1, "dataKeeper", data) << endl;

    auto ipcFd = ipc.GetFd();
    bool isRunning = true;
    fd_set fds;
    msg_t message;
    while (isRunning) {
        FD_ZERO(&fds);
        FD_SET(ipcFd, &fds);

        int ret = select(ipcFd + 1, &fds, 0, 0, nullptr);

        if ( ret == -1 ) {
            isRunning = false;
            return -1;
        } else if ( ret != 0 ) {
            ipc.Recv(message);

            MsgPack::package pckg(message.data.begin(), message.data.end());
            MsgPackVariant msgpack;
            msgpack.setPackage(pckg);

            bool ret = false;
            auto answer = msgpack.toMap(&ret);
            if (!ret) {
                cout << "Can't cast to map" << endl;
                break;
            }

            auto section = answer.at(asUserSection).toMap(&ret);
            auto bin = section["data"].toBin();
            string str(bin.begin(), bin.end());

            auto mids = section["mid"].toArray();

            for (auto& mid: mids) {
                cout << "mid: " << mid.toInt64() << endl;
            }
            isRunning = false;
        }
    }
    return 0;
}