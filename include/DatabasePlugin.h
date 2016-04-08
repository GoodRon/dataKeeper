/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef DATABASEPLUGIN_H
#define DATABASEPLUGIN_H

class AbstractConnection;

typedef AbstractConnection*(*pluginIface)(const char*);

extern "C" {

AbstractConnection* openConnection(const char* jsonConf);

}

#endif // DATABASEPLUGIN_H
