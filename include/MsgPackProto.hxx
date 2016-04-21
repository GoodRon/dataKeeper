/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef MSGPACKPROTO_HXX
#define MSGPACKPROTO_HXX

/**
 * @brief Перечисление параметров обязательной секции
 */
enum MsgPackKey {
    mppUserSection = -2,
    mppAdditionalSection = -1,
    mppPacketType = 0,
    mppSource = 1,
    mppDestination = 2,
    mppID = 3,
};

#endif // MSGPACKPROTO_HXX
