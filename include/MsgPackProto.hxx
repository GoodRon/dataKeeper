/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef MSGPACKPROTO_HXX
#define MSGPACKPROTO_HXX

/**
 * @brief Перечисление ключей обязательной секции
 */
enum MsgPackMandatorySection {
    msPacketType = 0,
    msID = 1
};

/**
 * @brief Перечисление ключей дополнительных секций
 */
enum MsgPackAdditional {
    asUserSection = -2,
    asServiceSection = -1
};

/**
 * @brief Перечисление ключей служебной дополнительной секции
 */
enum MsgPackServiceSection {
    ssSource = 0,
    ssDestination = 1
};

#endif // MSGPACKPROTO_HXX
