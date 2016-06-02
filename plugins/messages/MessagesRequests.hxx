/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef MESSAGE_REQUESTS_HXX
#define MESSAGE_REQUESTS_HXX

#include <string>

#include "MsgPack_types.h"

namespace messages_db {

/**
 * @brief Сформировать запрос на добавление сообщения в базу данных
 *
 * @param source тип источника сообщения
 * @param sa номер устройства-источника сообщения в системе
 * @param da номер устройства-приемника сообщения в системе
 * @param type тип данных сообщения
 * @param create_time время создания сообщения
 * @param io_time время приемки/отправки сообщения
 * @param exec_status признак обработки сообщения
 * @param status тип сообщения
 * @param channel имя канала связи
 * @param data данные сообщения
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package insertMessage(const std::string& source, int64_t sa, int64_t da, int32_t type,
                               int64_t create_time, int64_t io_time, bool exec_status, int32_t status,
                               const std::string& channel, const MsgPack::rawData& data,
                               const std::string& returnAddress = "");

/**
 * @brief Сформировать запрос на выдачу сообщения по его mid
 *
 * @param mid уникальный идентификатор сообщения
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package selectMessageByMid(int32_t mid, const std::string& returnAddress = "");

/**
 * @brief Сформировать запрос на выдачу mid сообщений согласно критериям поиска
 *
 * @param source тип источника сообщения ("" - не учитывается)
 * @param sa номер устройства-источника сообщения в системе (-1 - не учитывается)
 * @param da номер устройства-приемника сообщения в системе (-1 не учитывается)
 * @param type тип данных сообщения (-1 - не учитывается)
 * @param status тип сообщения (-1 - не учитывается)
 * @param channel имя канала связи ("" - не учитывается)
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package selectMessagesByParameters(const std::string& source, int64_t sa, int64_t da, int32_t type,
                                            int32_t status, const std::string& channel,
                                            const std::string& returnAddress = "");

/**
 * @brief Сформировать запрос на удаление всех сообщений
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package deleteAll(const std::string &returnAddress = "");

/**
 * @brief Сформировать запрос на удаление сообщения по его mid
 *
 * @param mid уникальный идентификатор сообщения
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package deleteMessage(int32_t mid, const std::string& returnAddress = "");

/**
 * @brief Сформировать запрос на удаление устаревших сообщений
 *
 * @param amount максимальное допустипое количество оставшихся наиболее свежих сообщений
 * @param source тип источника сообщения ("" - не учитывается)
 * @param sa номер устройства-источника сообщения в системе (-1 - не учитывается)
 * @param da номер устройства-приемника сообщения в системе (-1 не учитывается)
 * @param type тип данных сообщения (-1 - не учитывается)
 * @param status тип сообщения (-1 - не учитывается)
 * @param channel имя канала связи ("" - не учитывается)
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package deleteOldMessages(unsigned int amount, const std::string& source, int64_t sa,
                                   int64_t da, int32_t type, int32_t status,
                                   const std::string& channel, const std::string& returnAddress = "");

/**
 * @brief Сформировать запрос на обновление типа сообщения
 *
 * @param mid уникальный идентификатор сообщения
 * @param status новый тип
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package updateStatus(int32_t mid, int32_t status, const std::string& returnAddress = "");

/**
 * @brief Сформировать запрос на обновление канала связи сообщения
 *
 * @param mid уникальный идентификатор сообщения
 * @param channel новый канал связи
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package updateChannel(int32_t mid, const std::string& channel,
                               const std::string& returnAddress = "");

}

#endif // MESSAGE_REQUESTS_HXX
