/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef MESSAGESDB_HXX
#define MESSAGESDB_HXX

#include <string>

#include "AbstractConnection.hxx"

/**
 * @brief Класс соединения с базой данных messages
 **/
class MessagesConnection : public AbstractConnection {
public:
    /**
    * @brief Конструктор
    *
    * @param jsonConf содержание конфигурационного файла в формате json
    **/
    MessagesConnection(const std::string& jsonConf);

    /**
     * @overload
     */
    virtual ~MessagesConnection();

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
     * @brief Обработать запрос на добавление сообщения в базу данных
     *
     * @param request пакет запроса
     * @param answer сформированный ответ
     * @return bool
     */
    bool insertMessage(const MsgPack::MsgPackVariantMap& request,
                       MsgPack::MsgPackVariantMap& answer);

    /**
     * @brief Обработать запрос на выдачу сообщения по его mid
     *
     * @param request пакет запроса
     * @param answer сформированный ответ
     * @return bool
     */
    bool selectMessageByMid(const MsgPack::MsgPackVariantMap &request,
                            MsgPack::MsgPackVariantMap &answer);

    /**
     * @brief Обработать запрос на удаление всех сообщений
     *
     * @param request пакет запроса
     * @param answer сформированный ответ
     * @return bool
     */
    bool deleteAll(const MsgPack::MsgPackVariantMap& request,
                   MsgPack::MsgPackVariantMap& answer);

    /**
     * @brief Обработать запрос на удаление сообщения
     *
     * @param request пакет запроса
     * @param answer сформированный ответ
     * @return bool
     */
    bool deleteMessage(const MsgPack::MsgPackVariantMap& request,
                       MsgPack::MsgPackVariantMap& answer);

    /**
     * @brief Обработать запрос на удаление устаревших сообщений
     *
     * @param request пакет запроса
     * @param answer сформированный ответ
     * @return bool
     */
    bool deleteOldMessages(const MsgPack::MsgPackVariantMap& request,
                           MsgPack::MsgPackVariantMap& answer);

    /**
     * @brief Обработать запрос на выборку mid сообщений
     *
     * @param request пакет запроса
     * @param answer сформированный ответ
     * @return bool
     */
    bool selectMessagesByParameters(const MsgPack::MsgPackVariantMap& request,
                                    MsgPack::MsgPackVariantMap& answer);

    /**
     * @brief Обработать запрос на обновление статуса сообщения
     *
     * @param request пакет запроса
     * @param answer сформированный ответ
     * @return bool
     */
    bool updateStatus(const MsgPack::MsgPackVariantMap& request,
                      MsgPack::MsgPackVariantMap& answer);

    /**
     * @brief Обработать запрос на обновление канала связи для сообщения
     *
     * @param request пакет запроса
     * @param answer сформированный ответ
     * @return bool
     */
    bool updateChannel(const MsgPack::MsgPackVariantMap& request,
                       MsgPack::MsgPackVariantMap& answer);
};

#endif // MESSAGESDB_HXX
