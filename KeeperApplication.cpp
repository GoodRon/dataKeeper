/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <string>
#include <ipc_const.h>
#include <stdio.h>
#include <dlfcn.h>

#include <sys/select.h>
#include <jsoncpp/json/json.h>

#include "common.h"
#include "ipc_msgpack.h"
#include "MsgPackVariant.h"
#include "MsgPackVariantMap.h"
#include "MsgPack_unpack.h"
#include "KeeperApplication.h"
#include "ipc_fdnotify_recv.h"
#include "DbPluginHandler.hxx"
#include "AbstractConnection.hxx"

using namespace std;
using namespace ipc;
using namespace MsgPack;

const string ipcSock = "/tmp/keeper_ipc";

/**
 * @brief Перечисление параметров обязательной секции
 */
enum MsgPackKey {
    mpkDatabase = 0
};

bool KeeperApplication::loadDatabasePlugins(const string& jsonConf) {
    ifstream jsonFile;
    jsonFile.open(jsonConf);
    if (!jsonFile.good()) {
//        cerr << "Can't open json file " << jsonConf << endl;
        return false;
    }

    string line;
    string jsonContent;
    while (getline(jsonFile, line)) {
        jsonContent += line;
    }

    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(jsonContent, root)) {
//        cerr << "Can't parse json from " << jsonConf << endl;
        return false;
    }

    Json::Value plugins = root["plugins"];
    for (int index = 0; static_cast<size_t>(index) < plugins.size(); ++index) {
        DbPluginHandler plugin;
        plugin.jsonConf = plugins[index].asString();
        plugin.database = plugins[index].get("database", "").asString();
        plugin.type = plugins[index].get("type", "").asString();

        // TODO полный путь
        plugin.path = plugin.database + "_" + plugin.type + ".so";

        if (loadDatabasePlugin(plugin)) {
            m_databasePlugins[plugin.database] = plugin;
        }
    }
    return true;
}

void KeeperApplication::unloadDatabasePlugins() {
    for (auto& connection: m_databasePlugins) {
        // TODO check return value
        dlclose(connection.second.handle);
    }
    m_databasePlugins.clear();
}

int KeeperApplication::execute() noexcept {
    m_isRunning = true;

    fd_set fds;
//    timeval tv;
    msg_t message;

    while (m_isRunning) {
        FD_ZERO(&fds);
        FD_SET(m_ipcFd, &fds);

        int ret = select(m_ipcFd + 1, &fds, 0, 0, nullptr);

        if ( ret == -1 ) {
            m_isRunning = false;
            return -1;
        } else if ( ret != 0 ) {
            m_ipc->Recv(message);
            processIpcMsg(message);
        }
    }
    return m_returnCode;
}

void KeeperApplication::brake() {
    m_isRunning = false;
}

KeeperApplication::KeeperApplication() :
        m_isRunning(false),
        m_returnCode(0),
        m_ipc(new fdnotify_recv(ipcSock.c_str(), "dataKeeper")),
        m_ipcFd(m_ipc->GetFd()),
        m_databasePlugins(),
        m_connectionsCache() {
}

KeeperApplication::~KeeperApplication() {
    unloadDatabasePlugins();
}

bool KeeperApplication::loadDatabasePlugin(DbPluginHandler& plugin) {
    plugin.handle = dlopen(plugin.path.c_str(), RTLD_LAZY);
    if (!plugin.handle) {
        return false;
    }

    plugin.connectionInstantiator = reinterpret_cast<pluginIface>(
            dlsym(plugin.handle, "returnDatabase"));
    if (!plugin.connectionInstantiator) {
        return false;
    }
    return true;
}

bool KeeperApplication::openConnection(const std::string& database) {
    // Если соединение уже есть в кэше
    if (m_connectionsCache.find(database) == m_connectionsCache.end()) {
        return true;
    }

    // Если нет плагина для этой базы данных
    if (m_databasePlugins.find(database) == m_databasePlugins.end()) {
        return false;
    }

    auto handler = m_databasePlugins[database];
    AbstractConnection* connection = handler.connectionInstantiator(handler.jsonConf.c_str());
    if (!connection) {
        return false;
    }

    shared_ptr<AbstractConnection> ptr(connection);
    m_connectionsCache[database] = ptr;
    return true;
}

void KeeperApplication::processIpcMsg(const ipc::msg_t& msg) {
    switch (msg.cmd) {
        case IpcCmd_Msgpack: {
            // Транслируем пришедшую по IPC последовательность байт в мэп значений MsgPack
            MsgPack::package pckg(msg.data.begin(), msg.data.end());
            MsgPack::MsgPackVariant msgpack;
            msgpack.setPackage(pckg);

            bool result = false;
            auto msgMap = msgpack.toMap(&result);
            // Если не можем преобразовать к мэпу - игнорируем
            if (!result) {
                break;
            }

            // Диспетчеризация сообщения к соответствующему плагину
            dispatchMsg(msgMap);
        } break;
        default:
            break;
    }
}

void KeeperApplication::dispatchMsg(const MsgPackVariantMap& request) {
    // Если нет обязательного параметра БД - игнорируем
    if (!request.contain(static_cast<int>(mpkDatabase))) {
        return;
    }

    auto value = request.at(mpkDatabase);
    string database = value.toString();

    if (!openConnection(database)) {
        return;
    }

    MsgPackVariantMap answer;
    m_connectionsCache[database]->processQuery(request, answer);

    if (answer.empty()) {
        return;
    }

    // TODO возврат ответа
}
