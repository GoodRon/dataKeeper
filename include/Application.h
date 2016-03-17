/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>

/**
 * @brief Абстрактный класс приложения
 **/
class Application {
public:
    /**
     * @brief Конструктор
     **/
    Application() : m_returnCode(0) {}

    // NOTE пока запретил копирование и присваивание. может и зря, посмотрим
    /**
     * @brief Конструктор копирования
     *
     * @param other
     **/
    Application(const Application& other) = delete;

    /**
     * @brief Оператор присваивания
     *
     * @param other
     * @return Application&
     **/
    Application& operator=(const Application& other) = delete;

    /**
     * @brief Деструктор
     **/
    virtual ~Application() {}

    /**
     * @brief Запустить приложение
     * Обертка, вызывает функцию logic(), реализующую
     * логику приложения
     *
     * @return int код возврата
     **/
    int execute() {
        logic();
        return m_returnCode;
    }

protected:
    /**
     * @brief Логика приложения
     **/
    virtual void logic() = 0;

protected:
    /**
	 * @brief Код возврата
	 */
    int m_returnCode;
};

#endif // APPLICATION_H