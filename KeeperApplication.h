/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef KEEPERAPPLICATION_H
#define KEEPERAPPLICATION_H

#include "Application.h"

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
};

#endif // KEEPERAPPLICATION_H
