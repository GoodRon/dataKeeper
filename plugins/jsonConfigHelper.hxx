/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef DATAKEEPER_JSONCONFIGHELPER_HXX
#define DATAKEEPER_JSONCONFIGHELPER_HXX

#include <string>
#include <vector>

bool jsonToCmdLine(const std::string& json, std::vector<std::string>& args);

#endif //DATAKEEPER_JSONCONFIGHELPER_HXX
