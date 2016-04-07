/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef DBPLUGINHANDLER_HXX
#define DBPLUGINHANDLER_HXX

#include <string>
#include <functional>

#include "DatabasePlugin.h"

struct DbPluginHandler {
    std::string path;

    void *handle;

    std::function<AbstractConnection*(const char*)> connectionInstantiator;
};

#endif // DBPLUGINHANDLER_HXX
