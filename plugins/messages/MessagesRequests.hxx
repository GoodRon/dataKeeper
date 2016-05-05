/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef MESSAGE_REQUESTS_HXX
#define MESSAGE_REQUESTS_HXX

#include <string>

#include "types.h"

namespace messages {

/**
 * @brief Сформировать запрос на добавление сообщения в базу данных
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
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
 * @return MsgPack::package
 **/
MsgPack::package insertMessage(const std::string &returnAddress, const std::string &source,
                               uint64_t sa, uint64_t da, int32_t type, int64_t create_time,
                               int64_t io_time, bool exec_status, int32_t status,
                               const std::string &channel, const MsgPack::rawData &data);

/**
 * @brief Сформировать запрос на выдачу сообщения по его mid
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @param mid уникальный идентификатор сообщения
 * @return MsgPack::package
 **/
MsgPack::package selectMessageByMid(const std::string &returnAddress, int64_t mid);

/**
 * @brief Сформировать запрос на выдачу mid сообщений согласно критериям поиска
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @param source тип источника сообщения ("" - не учитывается)
 * @param sa номер устройства-источника сообщения в системе (-1 - не учитывается)
 * @param da номер устройства-приемника сообщения в системе (-1 не учитывается)
 * @param type тип данных сообщения (-1 - не учитывается)
 * @param status тип сообщения (-1 - не учитывается)
 * @param channel имя канала связи ("" - не учитывается)
 * @return MsgPack::package
 **/
MsgPack::package selectMessagesByParameters(const std::string &returnAddress,
                                            const std::string &source = "", int64_t sa = -1,
                                            int64_t da = -1, int32_t type = -1, int32_t status = -1,
                                            const std::string &channel = "");

/**
 * @brief Сформировать запрос на удаление всех сообщений
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @return MsgPack::package
 **/
MsgPack::package deleteAll(const std::string &returnAddress);

/**
 * @brief Сформировать запрос на удаление сообщения по его mid
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @param mid уникальный идентификатор сообщения
 * @return MsgPack::package
 **/
MsgPack::package deleteMessage(const std::string &returnAddress, int64_t mid);

/**
 * @brief Сформировать запрос на удаление устаревших сообщений
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @param amount максимальное допустипое количество оставшихся наиболее свежих сообщений
 * @param source тип источника сообщения ("" - не учитывается)
 * @param sa номер устройства-источника сообщения в системе (-1 - не учитывается)
 * @param da номер устройства-приемника сообщения в системе (-1 не учитывается)
 * @param type тип данных сообщения (-1 - не учитывается)
 * @param status тип сообщения (-1 - не учитывается)
 * @param channel имя канала связи ("" - не учитывается)
 * @return MsgPack::package
 **/
MsgPack::package deleteOldMessages(const std::string &returnAddress, unsigned amount = 0,
                                   const std::string &source = "", int64_t sa = -1,
                                   int64_t da = -1, int32_t type = -1, int32_t status = -1,
                                   const std::string &channel = "");

/**
 * @brief Сформировать запрос на обновление типа сообщения
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @param mid уникальный идентификатор сообщения
 * @param status новый тип
 * @return MsgPack::package
 **/
MsgPack::package updateStatus(const std::string &returnAddress, int64_t mid, int32_t status);

/**
 * @brief Сформировать запрос на обновление канала связи сообщения
 *
 * @param returnAddress обратный адрес в системе IPC для ответного сообщения
 * @param mid уникальный идентификатор сообщения
 * @param channel новый канал связи
 * @return MsgPack::package
 **/
MsgPack::package updateChannel(const std::string &returnAddress, int64_t mid,
                               const std::string &channel);

}

#endif // MESSAGE_REQUESTS_HXX
