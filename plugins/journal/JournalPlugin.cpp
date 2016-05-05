/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include "DatabasePlugin.h"
#include "JournalConnection.hxx"

extern "C" {

/**
 * @brief Открыть соединение с базой данных journal
 *
 * @param jsonConf
 * @return AbstractConnection*
 **/
AbstractConnection* openConnection(const char *jsonConf) {
    return new JournalConnection(jsonConf);
}

}
