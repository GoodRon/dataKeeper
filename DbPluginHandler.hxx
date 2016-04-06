/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef DBPLUGINHANDLER_HXX
#define DBPLUGINHANDLER_HXX

#include <string>
#include <functional>

class database;

typedef database*(*pluginIface)(int, char**);

struct DbPluginHandler {
    std::string path;

    void *handle;

    std::function<database*(int, char**)> databaseInstantiator;
};

#endif // DBPLUGINHANDLER_HXX
