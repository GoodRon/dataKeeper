/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include "DatabasePlugin.h"
#include "MessagesDb.hxx"

extern "C" {

AbstractConnection* openConnection(const char *jsonConf) {
    return new MessagesDb(jsonConf);
}

}
