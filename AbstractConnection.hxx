/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef ABSTRACT_CONNECTION_HXX
#define ABSTRACT_CONNECTION_HXX

#include <string>

class MsgPackVariantMap;

class AbstractConnection {
public:
    AbstractConnection(const std::string& jsonConf): m_jsonConf(jsonConf) {}

    virtual ~AbstractConnection() = 0;

    virtual bool processQuery(const MsgPackVariantMap& request, MsgPackVariantMap& answer) = 0;

private:
    std::string m_jsonConf;
};

#endif // ABSTRACT_CONNECTION_HXX
