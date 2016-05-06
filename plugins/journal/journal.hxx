/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef JOURNAL_HXX
#define JOURNAL_HXX

#include <stdint.h>

#include <string>

#include <odb/core.hxx>

#pragma db object
/**
 * @brief Класс записи для базы данных journal
 **/
class Journal {
public:
    /**
	 * @brief Конструктор
     *
     * @param sa номер источника в системе
     * @param da номер приемника в системе
     * @param toper время совершения операции
     * @param oper описание операции
	 */
    Journal(int32_t sa, int32_t da, time_t toper, const std::string& oper):
            m_mid(0), m_sa(sa), m_da(da), m_toper(toper), m_oper(oper) {
    }

    /**
     * @brief Деструктор
     **/
    ~Journal() {}

    /**
     * @brief Вернуть ID сообщения в базе данных
     *
     * @return int32_t
     **/
    int32_t getMid() const {
        return m_mid;
    }

    /**
     * @brief Вернуть источник
     *
     * @return int32_t
     **/
    int32_t getSA() const {
        return m_sa;
    }

    /**
     * @brief Вернуть приемник
     *
     * @return int32_t
     **/
    int32_t getDA() const {
        return m_da;
    }

    /**
     * @brief Вернуть время операции
     *
     * @return time_t
     **/
    time_t getToper() const {
        return m_toper;
    }

    /**
     * @brief Вернуть описание операции
     *
     * @return string
     **/
    std::string getOper() const {
        return m_oper;
    }

private:
    friend class odb::access;

    /**
     * @brief Конструктор по умолчанию
     */
    Journal() {}

#pragma db id auto
    /**
     * @brief ID в базе
     */
    int32_t m_mid;

    /**
     * @brief Источник
     */
    int32_t m_sa;

    /**
     * @brief Приемник
     */
    int32_t m_da;

    /**
     * @brief Время совершения операции
     */
    time_t m_toper;

    /**
     * @brief Описание операции
     */
    std::string m_oper;
};

#endif // JOURNAL_HXX
