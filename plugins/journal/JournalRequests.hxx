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
 * @param sa источник
 * @param da приемник
 * @param toper время совершения операции
 * @param oper описание операции
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package insertRecord(int32_t sa, int32_t da, time_t toper, const std::string& oper,
                              const std::string& returnAddress = "");

/**
 * @brief Сформировать запрос на удаление всех записей
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package deleteAll(const std::string &returnAddress = "");

/**
 * @brief Сформировать запрос на удаление устаревших записей
 *
 * @param amount допустимое количество наиболее свежих записей в базе
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package deleteOldRecords(unsigned int amount, const std::string& returnAddress);

/**
 * @brief Сформировать параметризированный запрос на удаление устаревших записей
 *
 * @param sa источник (-1 - не учитывается)
 * @param da приемник (-1 - не учитывается)
 * @param topermax (0 - не учитывается)
 * @param topermin (0 - не учитывается)
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package selectRecordsByParameters(int32_t sa, int32_t da, time_t topermax, time_t topermin,
                                           const std::string& returnAddress = "");

/**
 * @brief Сформировать запрос на выдачу записи по mid
 *
 * @param mid уникальный идентификатор записи
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package selectRecordByMid(int64_t mid, const std::string& returnAddress = "");

}

#endif // JOURNAL_REQUESTS_HXX
