/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef JOURNAL_REQUESTS_HXX
#define JOURNAL_REQUESTS_HXX

#include <string>

#include "MsgPack_types.h"

namespace journal_db {

MsgPack::package insertRecord(const std::string &returnAddress, int32_t sa,
                              int32_t da, time_t toper, const std::string &oper);

/**
 * @brief Сформировать запрос на удаление всех записей
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package deleteAll(const std::string &returnAddress);

MsgPack::package deleteOldRecords(const std::string &returnAddress, unsigned amount = 0);

MsgPack::package selectRecordsByParameters(const std::string &returnAddress,
                                           int32_t sa = -1, int32_t da = -1,
                                           time_t topermax = 0, time_t topermin = 0);

MsgPack::package selectRecordByMid(const std::string &returnAddress, int64_t mid);

}

#endif // JOURNAL_REQUESTS_HXX
