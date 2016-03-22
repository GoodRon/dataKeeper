/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <string>

#include <sys/select.h>

#include "KeeperApplication.h"
#include "ipc_fdnotify_recv.h"

using namespace std;
using namespace ipc;

const string ipcSock = "/tmp/keeper_ipc";

int KeeperApplication::execute() noexcept {
    m_isRunning = true;

    fd_set fds;
    timeval tv;
    while (m_isRunning) {
        FD_ZERO(&fds);
        FD_SET(m_ipcFd, &fds);

        int ret = select(m_ipcFd + 1, &fds, 0, 0, nullptr);

        if ( ret == -1 ) {
            m_isRunning = false;
            return -1;
        } else if ( ret != 0 ) {
            msg_t message;
            m_ipc->Recv(message);
            processIpcMsg(message);
        }
    }

    return m_returnCode;
}

KeeperApplication::KeeperApplication() :
        m_isRunning(false),
        m_returnCode(0),
        m_ipc(new fdnotify_recv(ipcSock.c_str(), "dataKeeper")),
        m_ipcFd(m_ipc->GetFd()) {
}

KeeperApplication::~KeeperApplication() {
}

void KeeperApplication::processIpcMsg(const ipc::msg_t& data) {

}

void KeeperApplication::dispatchMsgPack(const package &data) {

}
