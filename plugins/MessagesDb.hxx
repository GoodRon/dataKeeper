/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef MESSAGESDB_HXX
#define MESSAGESDB_HXX

#include <string>

#include "AbstractConnection.hxx"

class MessagesDb: public AbstractConnection {
public:
    MessagesDb(const std::string& jsonConf);

    virtual ~MessagesDb();

    virtual bool processQuery(const MsgPack::MsgPackVariantMap& request,
                              MsgPack::MsgPackVariantMap& answer);

protected:
    virtual void instantiateDatabase();
};

#endif // MESSAGESDB_HXX
