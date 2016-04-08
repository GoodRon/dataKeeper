/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef ABSTRACT_CONNECTION_HXX
#define ABSTRACT_CONNECTION_HXX

#include <string>
#include <memory>

namespace MsgPack {
    class MsgPackVariantMap;
}

namespace odb {
    class database;
}

class AbstractConnection {
public:
    AbstractConnection(const std::string& jsonConf): m_jsonConf(jsonConf) {}

    virtual ~AbstractConnection() = 0;

    virtual bool processQuery(const MsgPack::MsgPackVariantMap& request,
                              MsgPack::MsgPackVariantMap& answer) = 0;

protected:
    std::string m_jsonConf;

    std::shared_ptr<odb::database> m_database;
};

#endif // ABSTRACT_CONNECTION_HXX
