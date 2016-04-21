/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef DBPLUGINHANDLER_HXX
#define DBPLUGINHANDLER_HXX

#include <string>
#include <functional>

#include "DatabasePlugin.h"

typedef std::function<AbstractConnection*(const char*)> ConnectionInstantiator;

/**
 * @brief Хендлер для плагина работы с базой данных
 **/
struct DbPluginHandler {
    /**
     * @brief Конструктор
     **/
    DbPluginHandler(): path(), database(), type(), jsonConf(), handle(nullptr),
                       connectionInstantiator() {}

    /**
     * @brief Полный путь к файлу плагина
     **/
    std::string path;

    /**
     * @brief Имя базы данных
     **/
    std::string database;

    /**
     * @brief Тип базы данных
     **/
    std::string type;

    /**
     * @brief Содержимое конфигурационного файла в формате json
     **/
    std::string jsonConf;

    /**
     * @brief Хэндлер для библиотеки
     **/
    void *handle;

    /**
     * @brief Функция для инстанциации соединения с базой данных
     **/
    ConnectionInstantiator connectionInstantiator;
};

#endif // DBPLUGINHANDLER_HXX
