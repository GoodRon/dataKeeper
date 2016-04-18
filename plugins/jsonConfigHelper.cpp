/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <jsoncpp/json/json.h>

#include "jsonConfigHelper.hxx"

bool sqliteParser(const std::string& jsonConf, std::vector<std::string>& args) {
    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(jsonConf, root)) {
        return false;
    }

    // Имя запускаемой программы
    args.push_back("dataKeeper");

    auto filename = root.get("filename", "").asString();
    if (filename != "") {
        args.push_back("--database");
        args.push_back(filename);
    }

    auto isNeedToCreate = root.get("create", "false").asBool();
    if (isNeedToCreate) {
        args.push_back("--create");
    }

    auto isReadOnly = root.get("read only", "false").asBool();
    if (isReadOnly) {
        args.push_back("--read-only");
    }

    auto optionsFile = root.get("options file", "").asString();
    if (optionsFile != "") {
        args.push_back("--options-file");
        args.push_back(optionsFile);
    }
    return true;
}

bool mysqlParser(const std::string& jsonConf, std::vector<std::string>& args) {
    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(jsonConf, root)) {
        return false;
    }

    // Имя запускаемой программы
    args.push_back("dataKeeper");

    auto user = root.get("user", "").asString();
    if (user != "") {
        args.push_back("--user");
        args.push_back(user);
    }

    auto password = root.get("password", "").asString();
    if (password != "") {
        args.push_back("--password");
        args.push_back(password);
    }

    auto database = root.get("database", "").asString();
    if (database != "") {
        args.push_back("--database");
        args.push_back(database);
    }

    auto host = root.get("host", "").asString();
    if (host != "") {
        args.push_back("--host");
        args.push_back(host);
    }

    auto port = root.get("port", "").asString();
    if (port != "") {
        args.push_back("--port");
        args.push_back(port);
    }

    auto socket = root.get("socket", "").asString();
    if (socket != "") {
        args.push_back("--socket");
        args.push_back(socket);
    }

    auto optionsFile = root.get("options file", "").asString();
    if (optionsFile != "") {
        args.push_back("--options-file");
        args.push_back(optionsFile);
    }
    return true;
}

// TODO write me
bool pgsqlParser(const std::string& jsonConf, std::vector<std::string>& args) {

    return true;
}

// TODO write me
bool mssqlParser(const std::string& jsonConf, std::vector<std::string>& args) {

    return true;
}

bool jsonToCmdLine(const std::string& jsonConf, std::vector<std::string>& args) {
    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(jsonConf, root)) {
        return false;
    }

    auto type = root.get("type", "").asString();
    if (type == "") {
        return false;
    }

    bool ret = false;

    if (type == "sqlite") {
        ret = sqliteParser(jsonConf, args);
    }

    if (type == "mysql") {
        ret = mysqlParser(jsonConf, args);
    }

    if (type == "pgsql") {
        ret = pgsqlParser(jsonConf, args);
    }
    return ret;
}