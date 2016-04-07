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
#include "MsgPackProto.hxx"

using namespace std;
using namespace ipc;
using namespace MsgPack;

const string ipcSock = "/tmp/keeper_ipc";

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
            package pckg(msg.data.begin(), msg.data.end());
            MsgPackVariant msgpack;
            msgpack.setPackage(pckg);

            bool ret = false;
            auto request = msgpack.toMap(&ret);
            // Если не можем преобразовать к мэпу - игнорируем
            if (!ret) {
                break;
            }

            // Обработка сообщения соответствующим плагином
            MsgPackVariantMap result;
            executeRequest(request, result);
            if (result.empty()) {
                break;
            }

            // Отправка ответа
            auto package = result.getPackage();
            busipc_client::RawData data(package.begin(), package.end());
            m_ipc->SendRep(msg.cmd, msg.id, msg.sa, data);
        } break;
        default:
            break;
    }
}

void KeeperApplication::executeRequest(const MsgPackVariantMap &message,
                                       MsgPackVariantMap &answer) {
    // Если тип пакета не тот - игнорируем
    if (!message.contain(static_cast<int>(mppPacketType))) {
        return;
    }

    auto value = message.at(mppPacketType);
    if (value.toString() != "Database") {
        return;
    }

    // Добываем служебную дополнительную секцию
    if (!message.contain(static_cast<int>(mppAdditionalSection))) {
        return;
    }

    value = message.at(mppAdditionalSection);
    bool ret = false;
    auto request = value.toMap(&ret);
    // Если не можем преобразовать к мэпу - игнорируем
    if (!ret) {
        return;
    }

    // Если не указана база данных - игнорируем
    if (!request.contain("database")) {
        return;
    }

    // Добываем название базы данных
    value = request.at("database");
    auto database = value.toString();

    // Открываем соединение
    if (!openConnection(database)) {
        return;
    }

    MsgPackVariantMap result;
    m_connectionsCache[database]->processQuery(message, result);

    // Ответа не будет
    if (result.empty()) {
        return;
    }

    // Формируем ответ
    answer[mppPacketType] = message[mppPacketType];
    if (message.contain(static_cast<int>(mppDestination))) {
        answer[mppSource] = message[mppDestination];
    }
    if (message.contain(static_cast<int>(mppSource))) {
        answer[mppDestination] = message[mppSource];
    }
    if (message.contain(static_cast<int>(mppSource))) {
        answer[mppID] = message[mppID];
    }
    answer[mppAdditionalSection] = MsgPackVariant(result);
}
