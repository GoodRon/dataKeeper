/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef JOURNAL_REQUESTS_HXX
#define JOURNAL_REQUESTS_HXX

#include <string>

#include "MsgPack_types.h"

namespace journal_db {

/**
 * @brief Сформировать запрос добавление записи
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @param sa источник
 * @param da приемник
 * @param toper время совершения операции
 * @param oper описание операции
 * @return MsgPack::package
 **/
MsgPack::package insertRecord(const std::string &returnAddress, int32_t sa,
                              int32_t da, time_t toper, const std::string &oper);

/**
 * @brief Сформировать запрос на удаление всех записей
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package deleteAll(const std::string &returnAddress);

/**
 * @brief Сформировать запрос на удаление устаревших записей
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @param amount допустимое количество наиболее свежих записей в базе
 * @return MsgPack::package
 **/
MsgPack::package deleteOldRecords(const std::string &returnAddress, unsigned amount = 0);

/**
 * @brief Сформировать параметризированный запрос на удаление устаревших записей
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @param sa источник (-1 - не учитывается)
 * @param da приемник (-1 - не учитывается)
 * @param topermax (0 - не учитывается)
 * @param topermin (0 - не учитывается)
 * @return MsgPack::package
 **/
MsgPack::package selectRecordsByParameters(const std::string &returnAddress,
                                           int32_t sa = -1, int32_t da = -1,
                                           time_t topermax = 0, time_t topermin = 0);

/**
 * @brief Сформировать запрос на выдачу записи по mid
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @param mid уникальный идентификатор записи
 * @return MsgPack::package
 **/
MsgPack::package selectRecordByMid(const std::string &returnAddress, int64_t mid);

}

#endif // JOURNAL_REQUESTS_HXX
