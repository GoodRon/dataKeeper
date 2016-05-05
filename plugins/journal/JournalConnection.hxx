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

    bool insertRecord(const MsgPack::MsgPackVariantMap& request,
                      MsgPack::MsgPackVariantMap& answer);

    bool deleteAll(const MsgPack::MsgPackVariantMap& request,
                   MsgPack::MsgPackVariantMap& answer);
};


#endif // JOURNAL_CONNECTION_HXX
