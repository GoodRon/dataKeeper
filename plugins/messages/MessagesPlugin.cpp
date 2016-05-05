/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include "DatabasePlugin.h"
#include "MessagesConnection.hxx"

extern "C" {

/**
 * @brief Открыть соединение с базой данных messages
 *
 * @param jsonConf
 * @return AbstractConnection*
 **/
AbstractConnection* openConnection(const char *jsonConf) {
    return new MessagesConnection(jsonConf);
}

}
