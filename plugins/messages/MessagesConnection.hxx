/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef MESSAGESDB_HXX
#define MESSAGESDB_HXX

#include <string>

#include "AbstractConnection.hxx"

/**
 * @brief Класс соединения с базой данных messages.db
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
     */
    bool insertMessage(const MsgPack::MsgPackVariantMap& request,
                       MsgPack::MsgPackVariantMap& answer);

    bool selectMessageByMid(const MsgPack::MsgPackVariantMap &request,
                            MsgPack::MsgPackVariantMap &answer);

    bool deleteAll(const MsgPack::MsgPackVariantMap& request,
                   MsgPack::MsgPackVariantMap& answer);

    bool deleteMessage(const MsgPack::MsgPackVariantMap& request,
                       MsgPack::MsgPackVariantMap& answer);

    bool deleteOldMessages(const MsgPack::MsgPackVariantMap& request,
                           MsgPack::MsgPackVariantMap& answer);

    bool selectMessageByParameters(const MsgPack::MsgPackVariantMap& request,
                                   MsgPack::MsgPackVariantMap& answer);
};

#endif // MESSAGESDB_HXX
