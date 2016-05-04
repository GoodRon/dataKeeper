/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef MESSAGEREQUESTS_HXX
#define MESSAGEREQUESTS_HXX

#include <string>

#include "types.h"

MsgPack::package insertMessage(const std::string& returnAddress, const std::string& source,
                               uint64_t sa, uint64_t da, int32_t type, int64_t create_time,
                               int64_t io_time, bool exec_status, int32_t status,
                               const std::string& channel, const MsgPack::rawData& data);

MsgPack::package selectMessageByMid(const std::string &returnAddress, int64_t mid);

MsgPack::package selectMessagesByParameters(const std::string& returnAddress,
                                            const std::string& source = "", int64_t sa = -1,
                                            int64_t da = -1, int32_t type = -1, int32_t status = -1,
                                            const std::string& channel = "");

MsgPack::package deleteAll(const std::string& returnAddress);

MsgPack::package deleteMessage(const std::string& returnAddress, int64_t mid);

MsgPack::package deleteOldMessages(const std::string& returnAddress, unsigned amount = 0,
                                   const std::string& source = "", int64_t sa = -1,
                                   int64_t da = -1, int32_t type = -1, int32_t status = -1,
                                   const std::string& channel = "");

MsgPack::package updateStatus(const std::string& returnAddress, int64_t mid, int32_t status);

MsgPack::package updateChannel(const std::string& returnAddress, int64_t mid,
                               const std::string& channel);

#endif // MESSAGEREQUESTS_HXX
