/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef DBPLUGINHANDLER_HXX
#define DBPLUGINHANDLER_HXX

#include <string>
#include <functional>

#include "DatabasePlugin.h"

typedef std::function<AbstractConnection*(const char*)> ConnectionInstantiator;

struct DbPluginHandler {
    DbPluginHandler(): path(), database(), type(), jsonConf(), handle(nullptr),
                       connectionInstantiator() {}

    std::string path;

    std::string database;

    std::string type;

    std::string jsonConf;

    void *handle;

    ConnectionInstantiator connectionInstantiator;
};

#endif // DBPLUGINHANDLER_HXX
