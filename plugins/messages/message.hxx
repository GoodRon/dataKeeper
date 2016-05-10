/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>
#include <stdint.h>

#include <odb/core.hxx>

#pragma db object
/**
 * @brief Класс сообщения для базы данных messages
 **/
class Message {
public:
    /**
	 * @brief Конструктор
     *
     * @param source тип источника сообщения
     * @param sa номер источника в системе
     * @param da номер приемника в системе
     * @param type тип данных сообщения
     * @param create_time время создания
     * @param io_time время приёма/отправки
     * @param exec_status признак обработки
     * @param status тип сообщения
     * @param channel имя канала связи
     * @param data данные
	 */
    Message(const std::string& source,
            int64_t sa, int64_t da, int32_t type,
            int64_t create_time, int64_t io_time,
            bool exec_status, int32_t status, const std::string& channel,
            const std::vector<char>& data):

            m_mid(0), m_source(source), m_sa(sa), m_da(da),
            m_type(type), m_create_time(create_time),
            m_io_time(io_time), m_exec_status(exec_status),
            m_status(status), m_channel(channel), m_data(data) {
    }

    /**
     * @brief Деструктор
     **/
    ~Message() {}

    /**
     * @brief Вернуть ID сообщения в базе данных
     *
     * @return unsigned long
     **/
    int32_t getMid() const {
        return m_mid;
    }

    /**
     * @brief Вернуть тип источника
     *
     * @return string
     **/
    std::string getSource() const {
        return m_source;
    }

    /**
     * @brief Вернуть источник
     *
     * @return unsigned long
     **/
    int64_t getSA() const {
        return m_sa;
    }

    /**
     * @brief Вернуть приемник
     *
     * @return unsigned long
     **/
    int64_t getDA() const {
        return m_da;
    }

    /**
     * @brief Вернуть тип сообщения
     *
     * @return int
     **/
    int32_t getType() const {
        return m_type;
    }

    /**
     * @brief Вернуть время создания
     *
     * @return unsigned long
     **/
    int64_t getCreateTime() const {
        return m_create_time;
    }

    /**
     * @brief Вернуть время приема/передачи
     *
     * @return unsigned long
     **/
    int64_t getIoTime() const {
        return m_io_time;
    }

    /**
     * @brief Вернуть признак обработки
     *
     * @return bool
     **/
    bool getExecStatus() const {
        return m_exec_status;
    }

    /**
     * @brief Вернуть статус сообщения
     *
     * @return int
     **/
    int32_t getStatus() const {
        return m_status;
    }

    /**
     * @brief Вернуть канал связи
     *
     * @return string
     **/
    std::string getChannel() const {
        return m_channel;
    }

    /**
     * @brief Вернуть данные
     *
     * @return vector<char>
     **/
    std::vector<char> getData() const {
        return m_data;
    }

    /**
     * @brief Изменить статус
     * @param status
     **/
    void setStatus(int32_t status) {
        m_status = status;
    }

    /**
     * @brief Изменить канал связи
     * @param channel
     **/
    void setChannel(const std::string& channel) {
        m_channel = channel;
    }

private:
    friend class odb::access;

    /**
     * @brief Конструктор по умолчанию
     */
    Message() {}

#pragma db id auto
    /**
     * @brief ID в базе
     */
    int32_t m_mid;

    /**
     * @brief Тип источника
     */
    std::string m_source;

    /**
     * @brief Источник
     */
    int64_t m_sa;

    /**
     * @brief Приемник
     */
    int64_t m_da;

    /**
     * @brief Тип данных
     */
    int32_t m_type;

    /**
     * @brief Время создания
     */
    int64_t m_create_time;

    /**
     * @brief Время приёма/отправки
     */
    int64_t m_io_time;

    /**
     * @brief Признак обработки
     */
    bool m_exec_status;

    /**
     * @brief Тип сообщения
     */
    int32_t m_status;

    /**
     * @brief Канал связи
     */
    std::string m_channel;

#ifdef DATABASE_PGSQL
 #pragma db type("BYTEA")
#else
 #pragma db type("BLOB")
#endif
    /**
     * @brief Данные
     */
    std::vector<char> m_data;
};

#endif // MESSAGE_H
