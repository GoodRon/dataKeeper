/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef KEEPERAPPLICATION_H
#define KEEPERAPPLICATION_H

#include <memory>
#include <atomic>

#include "ipc_const.h"

namespace ipc {
    class fdnotify_recv;
}

namespace MsgPack {
    class MsgPackVariantMap;
}

/**
 * @brief Класс приложения dataKeeper. Синглтон
 *
 **/
class KeeperApplication {
public:
    /**
     * @brief Метод для получения ссылки на объект синглтона
     *
     * @return KeeperApplication&
     */
    static KeeperApplication& getInstance() {
        static KeeperApplication instance;
        return instance;
    }

    /**
     * @brief Загрузить плагины для работы С БД из json-конфига
     *
     * @param jsonConf конфигурационный файл
     * @return bool
     **/
    bool loadDatabasePlugins(const std::string& jsonConf);

    /**
     * @brief Метод для запуска основной логики приложения
     *
     * @return int код возврата
     **/
    int execute() noexcept;

    /**
     * @brief Прервать выполнение приложения
     */
    void brake();

private:
    /**
     * @brief Конструктор
     **/
    KeeperApplication();

    /**
     * @brief Деструктор
     **/
    ~KeeperApplication();

    /**
     * @brief Конструктор копирования
     *
     * @param other
     **/
    KeeperApplication(const KeeperApplication& other) = delete;

    /**
     * @brief Оператор присваивания
     *
     * @param other
     * @return Application&
     **/
    KeeperApplication& operator=(const KeeperApplication& other) = delete;

    /**
     * @brief Обработать межпроцессное сообщение
     *
     * @param msg сообщение
     */
    void processIpcMsg(const ipc::msg_t& msg);

    /**
     * @brief Передать сообщение адресату
     *
     * @param data данные
     */
    void dispatchMsg(const MsgPack::MsgPackVariantMap& data);

private:
    /**
	 * @brief Флаг работы приложения
	 */
    std::atomic<bool> m_isRunning;

    /**
	 * @brief Код возврата
	 */
    int m_returnCode;

    /**
	 * @brief Объект IPC
	 */
    std::unique_ptr<ipc::fdnotify_recv> m_ipc;

    /**
	 * @brief Дескриптор оповещения о приходе межпроцессного сообщения
	 */
    int m_ipcFd;
};

#endif // KEEPERAPPLICATION_H