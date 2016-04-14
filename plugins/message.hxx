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
class Message {
public:
    /**
	 * @brief Конструктор
     *
     * @param source
     * @param sa
     * @param da
     * @param type
     * @param create_time
     * @param io_time
     * @param exec_status
     * @param status
     * @param channel
     * @param data
	 */
    Message(const std::string& source,
            unsigned long sa, unsigned long da, int type,
            unsigned long create_time, unsigned long io_time,
            bool exec_status, int status, const std::string& channel,
            const std::vector<uint8_t>& data):

            m_mid(), m_source(source), m_sa(sa), m_da(da),
            m_type(type), m_create_time(create_time),
            m_io_time(io_time), m_exec_status(exec_status),
            m_status(status), m_channel(channel), m_data(data) {
    }

    ~Message() {}

    unsigned long getMid() const {
        return m_mid;
    }

    int getStatus() const {
        return m_status;
    }

private:
    friend class odb::access;

    Message() {}

#pragma db id auto
    unsigned long m_mid;
    std::string m_source;
    unsigned long m_sa;
    unsigned long m_da;
    int m_type;
    unsigned long m_create_time;
    unsigned long m_io_time;
    bool m_exec_status;
    int m_status;
    std::string m_channel;
//
//#if defined(DATABASE_PGSQL)
//#pragma db type("BYTEA")
//#else
//#pragma db type("BLOB")
//#endif
    std::vector<uint8_t> m_data;
};

#endif // MESSAGE_H
