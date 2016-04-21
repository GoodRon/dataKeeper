/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef JSONCONFIGHELPER_HXX
#define JSONCONFIGHELPER_HXX

#include <string>
#include <vector>

/**
 * @brief Вспомогательная функция для перевода параметров конфигурационного
 * json-файла в аргументы командной строки
 *
 * @param jsonConf Содержимое конфигурационного файла в формате json
 * @param args вектор аргументов командной строки
 * @return bool
 **/
bool jsonToCmdLine(const std::string& jsonConf, std::vector<std::string>& args);

#endif // JSONCONFIGHELPER_HXX
