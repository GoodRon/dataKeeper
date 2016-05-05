/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef JOURNAL_REQUESTS_HXX
#define JOURNAL_REQUESTS_HXX

#include <string>

#include "types.h"

namespace journal {

MsgPack::package insertRecord(const std::string &returnAddress, int32_t sa,
                              int32_t da, time_t toper, const std::string &oper);

/**
 * @brief Сформировать запрос на удаление всех записей
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package deleteAll(const std::string &returnAddress);

}

#endif // JOURNAL_REQUESTS_HXX
