/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <string>
#include <ipc_const.h>
#include <stdio.h>
#include <dlfcn.h>
#include <syslog.h>

#include <sys/select.h>
#include <jsoncpp/json/json.h>

#include "common.h"
#include "ipc_msgpack.h"
#include "ipc_const.h"
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

bool KeeperApplication::loadDatabasePlugins(const string& jsonConf) {
    ifstream jsonFile;
    jsonFile.open(jsonConf);
    if (!jsonFile.good()) {
        syslog(LOG_ERR, "Can't open json file %s", jsonConf.c_str());
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
        syslog(LOG_ERR, "Can't parse json from %s", jsonConf.c_str());
        return false;
    }

    Json::Value plugins = root["plugins"];
    for (int index = 0; static_cast<size_t>(index) < plugins.size(); ++index) {
        syslog(LOG_INFO, "Part of json config:  %s", plugins[index].toStyledString().c_str());

        DbPluginHandler plugin;
        plugin.jsonConf = plugins[index].toStyledString();
        plugin.database = plugins[index].get("database", "").asString();
        plugin.type = plugins[index].get("type", "").asString();

        // TODO полный путь
        plugin.path = plugins[index].get("dir", "").asString() +
                "lib" + plugin.database + "db_" + plugin.type + ".so";

        if (loadDatabasePlugin(plugin)) {
            m_databasePlugins[plugin.database] = plugin;
        }
    }
    return true;
}

void KeeperApplication::unloadDatabasePlugins() {
    for (auto& connection: m_databasePlugins) {
        // TODO check the return value
        dlclose(connection.second.handle);
    }
    m_databasePlugins.clear();
}

int KeeperApplication::execute() noexcept {
    m_isRunning = true;

    fd_set fds;
    msg_t message;

    while (m_isRunning) {
        FD_ZERO(&fds);
        FD_SET(m_ipcFd, &fds);

        int ret = select(m_ipcFd + 1, &fds, 0, 0, nullptr);

        if (ret == -1) {
            m_isRunning = false;
            m_returnCode = -1;
        } else if (ret != 0) {
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
        m_ipc(new fdnotify_recv(SOCK_DEFAULT, "dataKeeper")),
        m_ipcFd(m_ipc->GetFd()),
        m_databasePlugins(),
        m_connectionsCache() {
    openlog("dataKeeper", LOG_CONS, LOG_LOCAL3);
    setlogmask(LOG_UPTO(7));
}

KeeperApplication::~KeeperApplication() {
    unloadDatabasePlugins();
    closelog();
}

bool KeeperApplication::loadDatabasePlugin(DbPluginHandler& plugin) {
    plugin.handle = dlopen(plugin.path.c_str(), RTLD_LAZY);
    if (!plugin.handle) {
        syslog(LOG_ERR, "Can't dlopen %s, error %s", plugin.path.c_str(), dlerror());
        return false;
    }

    plugin.connectionInstantiator = reinterpret_cast<pluginIface>(
            dlsym(plugin.handle, "openConnection"));
    if (!plugin.connectionInstantiator) {
        syslog(LOG_ERR, "Can't instantiate plugin, error %s", dlerror());
        return false;
    }
    return true;
}

bool KeeperApplication::openConnection(const std::string& database) {
    // Если соединение уже есть в кэше
    if (m_connectionsCache.find(database) != m_connectionsCache.end()) {
        syslog(LOG_INFO, "%s already in cache", database.c_str());
        return true;
    }

    // Если нет плагина для этой базы данных
    if (m_databasePlugins.find(database) == m_databasePlugins.end()) {
        syslog(LOG_ERR, "%s has no plugin loaded", database.c_str());
        return false;
    }

    auto handler = m_databasePlugins[database];
    AbstractConnection* connection = handler.connectionInstantiator(handler.jsonConf.c_str());
    if (!connection) {
        syslog(LOG_ERR, "Can't instantiate connection for %s", database.c_str());
        return false;
    }

    shared_ptr<AbstractConnection> ptr(connection);
    m_connectionsCache[database] = ptr;
    return true;
}

void KeeperApplication::processIpcMsg(const ipc::msg_t& msg) {
    syslog(LOG_INFO, "Message has been received");

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
                syslog(LOG_ERR, "Can't cast message to map");
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

// TODO alert about bad packet
void KeeperApplication::executeRequest(const MsgPackVariantMap &message,
                                       MsgPackVariantMap &answer) {
    // Если тип пакета не тот - игнорируем
    if (!message.contain(static_cast<int>(msPacketType))) {
        syslog(LOG_ERR, "Wrong package type");
        return;
    }

    if (message.at(msPacketType).toString() != "Database") {
        syslog(LOG_ERR, "Wrong package type");
        return;
    }

    // Добываем служебную дополнительную секцию
    if (!message.contain(static_cast<int>(asServiceSection))) {
        syslog(LOG_ERR, "There is no service section in package");
        return;
    }

    auto valueService = message.at(asServiceSection);
    bool ret = false;
    auto address = valueService.toMap(&ret);
    // Если не можем преобразовать к мэпу - игнорируем
    if (!ret) {
        syslog(LOG_ERR, "Can't cast service section to map");
        return;
    }

    // Добываем пользовательскую секцию
    if (!message.contain(static_cast<int>(asUserSection))) {
        syslog(LOG_ERR, "There is no user section in package");
        return;
    }

    auto valueUser = message.at(asUserSection);
    ret = false;
    auto request = valueUser.toMap(&ret);
    // Если не можем преобразовать к мэпу - игнорируем
    if (!ret) {
        syslog(LOG_ERR, "Can't cast user section to map");
        return;
    }

    // Если не указана база данных - игнорируем
    if (!request.contain("database")) {
        syslog(LOG_ERR, "There is no database in package");
        return;
    }

    // Добываем название базы данных
    auto database = request.at("database").toString();

    // Открываем соединение
    if (!openConnection(database)) {
        syslog(LOG_ERR, "Can't open connection to %s", database.c_str());
        return;
    }

    MsgPackVariantMap result;
    m_connectionsCache[database]->processQuery(request, result);

    // Ответа не будет
    if (result.empty()) {
        return;
    }

    // Формируем ответ
    answer[msPacketType] = message[msPacketType];
    answer[msID] = message[msID];

    MsgPackVariantMap answerAddress;
    answerAddress[ssSource] = address.at(ssDestination).toString();
    answerAddress[ssDestination] = address.at(ssSource).toString();

    answer[asServiceSection] = answerAddress;
    answer[asUserSection] = MsgPackVariant(result);
}
