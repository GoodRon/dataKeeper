/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef KEEPERAPPLICATION_H
#define KEEPERAPPLICATION_H

#include <memory>

#include "Application.h"

namespace ipc {
    class fdnotify_recv;
}

/**
 * @brief Класс приложения dataKeeper
 *
 **/
class KeeperApplication: public Application {
public:
    /**
     * @brief Конструктор
     **/
    KeeperApplication();

    /**
     * @brief Деструктор
     **/
    virtual ~KeeperApplication();

protected:
    /**
     * @brief Логика приложения
     **/
    virtual void logic();

private:
    std::unique_ptr<ipc::fdnotify_recv> m_ipc;

    bool m_isRunning;
};

#endif // KEEPERAPPLICATION_H
