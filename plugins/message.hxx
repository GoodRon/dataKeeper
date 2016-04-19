/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>

#include <odb/core.hxx>

#pragma db object
/**
 * @brief Класс сообщения для базы данных messages.db
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
            unsigned long sa, unsigned long da, int type,
            unsigned long create_time, unsigned long io_time,
            bool exec_status, int status, const std::string& channel,
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
    unsigned long getMid() const {
        return m_mid;
    }

    std::string getSource() const {
        return m_source;
    }

    unsigned long getSA() const {
        return m_sa;
    }

    unsigned long getDA() const {
        return m_da;
    }

    int getType() const {
        return m_type;
    }

    unsigned long getCreateTime() const {
        return m_create_time;
    }

    unsigned long getIoTime() const {
        return m_io_time;
    }

    bool getExecStatus() const {
        return m_exec_status;
    }

    /**
     * @brief Вернуть статус сообщения
     *
     * @return int
     **/
    int getStatus() const {
        return m_status;
    }

    std::string getChannel() const {
        return m_channel;
    }

    std::vector<char> getData() const {
        return m_data;
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
    unsigned long m_mid;

    /**
     * @brief Тип источника
     */
    std::string m_source;

    /**
     * @brief Источник
     */
    unsigned long m_sa;

    /**
     * @brief Приемник
     */
    unsigned long m_da;

    /**
     * @brief Тип данных
     */
    int m_type;

    /**
     * @brief Время создания
     */
    unsigned long m_create_time;

    /**
     * @brief Время приёма/отправки
     */
    unsigned long m_io_time;

    /**
     * @brief Признак обработки
     */
    bool m_exec_status;

    /**
     * @brief Тип сообщения
     */
    int m_status;

    /**
     * @brief Канал связи
     */
    std::string m_channel;
//
//#if defined(DATABASE_PGSQL)
//#pragma db type("BYTEA")
//#else
//#pragma db type("BLOB")
//#endif
    /**
     * @brief Данные
     */
    std::vector<char> m_data;
};

#endif // MESSAGE_H
