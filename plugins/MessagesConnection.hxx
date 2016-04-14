/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef MESSAGESDB_HXX
#define MESSAGESDB_HXX

#include <string>

#include "AbstractConnection.hxx"

class MessagesConnection : public AbstractConnection {
public:
    MessagesConnection(const std::string& jsonConf);

    virtual ~MessagesConnection();

    virtual bool processQuery(const MsgPack::MsgPackVariantMap& request,
                              MsgPack::MsgPackVariantMap& answer);

protected:
    virtual void instantiateDatabase();

private:
    bool insertMessage(const MsgPack::MsgPackVariantMap& request);
};

#endif // MESSAGESDB_HXX
