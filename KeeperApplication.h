/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef KEEPERAPPLICATION_H
#define KEEPERAPPLICATION_H

#include <memory>

#include "ipc_fdnotify_recv.h"

class package;

/**
 * @brief Класс приложения dataKeeper
 *
 **/
class KeeperApplication {
public:
    static KeeperApplication& getInstance() {
        static KeeperApplication instance;
        return instance;
    }

    int execute() noexcept;

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

    void processIpcMsg(const ipc::msg_t& data);

    void dispatchMsgPack(const package& data);

private:
    bool m_isRunning;

    /**
	 * @brief Код возврата
	 */
    int m_returnCode;

    std::unique_ptr<ipc::fdnotify_recv> m_ipc;

    int m_ipcFd;
};

#endif // KEEPERAPPLICATION_H
