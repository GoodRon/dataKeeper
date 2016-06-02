/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef JOURNAL_CONNECTION_HXX
#define JOURNAL_CONNECTION_HXX

#include <string>

#include "AbstractConnection.hxx"

/**
 * @brief Класс соединения с базой данных journal
 **/
class JournalConnection : public AbstractConnection {
public:
    /**
    * @brief Конструктор
    *
    * @param jsonConf содержание конфигурационного файла в формате json
    **/
    JournalConnection(const std::string& jsonConf);

    /**
     * @overload
     */
    virtual ~JournalConnection();

    /**
     * @overload
     */
    virtual bool processQuery(const MsgPack::MsgPackVariantMap& request,
                              MsgPack::MsgPackVariantMap& answer);

protected:
    /**
     * @overload
     */
    virtual void instantiateDatabase();

private:
    /**
     * @brief Обработать запрос на добавление записи
     *
     * @param request пакет запроса
     * @param answer сформированный ответ
     * @return bool
     */
    bool insertRecord(const MsgPack::MsgPackVariantMap& request,
                      MsgPack::MsgPackVariantMap& answer);

    /**
     * @brief Обработать запрос на удаление всех записей
     *
     * @param request пакет запроса
     * @param answer сформированный ответ
     * @return bool
     */
    bool deleteAll(const MsgPack::MsgPackVariantMap& request,
                   MsgPack::MsgPackVariantMap& answer);

    /**
     * @brief Обработать запрос на удаление устаревших записей
     *
     * @param request пакет запроса
     * @param answer сформированный ответ
     * @return bool
     */
    bool deleteOldRecords(const MsgPack::MsgPackVariantMap& request,
                          MsgPack::MsgPackVariantMap& answer);

    /**
     * @brief Обработать параметризированный запрос на удаление устаревших записей
     *
     * @param request пакет запроса
     * @param answer сформированный ответ
     * @return bool
     */
    bool selectRecordsByParameters(const MsgPack::MsgPackVariantMap& request,
                                   MsgPack::MsgPackVariantMap& answer);

    /**
     * @brief Обработать запрос на выдачу записи по mid
     *
     * @param request пакет запроса
     * @param answer сформированный ответ
     * @return bool
     */
    bool selectRecordByMid(const MsgPack::MsgPackVariantMap& request,
                           MsgPack::MsgPackVariantMap& answer);
};


#endif // JOURNAL_CONNECTION_HXX
