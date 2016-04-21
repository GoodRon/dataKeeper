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

/**
 * @brief Абстрактный класс соединения с базой данных
 **/
class AbstractConnection {
public:
    /**
     * @brief Конструктор
     *
     * @param jsonConf содержание конфигурационного файла в формате json
     **/
    AbstractConnection(const std::string& jsonConf): m_jsonConf(jsonConf),
                                                     m_database() {}

    /**
     * @brief Деструктор
     **/
    virtual ~AbstractConnection() {}

    /**
     * @brief Обработать пришедший к базе данных запрос
     *
     * @param request запрос
     * @param answer ответ
     * @return bool
     **/
    virtual bool processQuery(const MsgPack::MsgPackVariantMap& request,
                              MsgPack::MsgPackVariantMap& answer) = 0;

protected:
    /**
     * @brief Инстанциировать объект базы данных
     **/
    virtual void instantiateDatabase() = 0;

protected:
    /**
     * @brief Содержание конфигурационного файла в формате json
     **/
    std::string m_jsonConf;

    /**
     * @brief Объект для работы с базой данных
     **/
    std::shared_ptr<odb::database> m_database;
};

#endif // ABSTRACT_CONNECTION_HXX
