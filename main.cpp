/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <iostream>

#include "KeeperApplication.h"

using namespace std;

int main() {
    cout << "Hello, Keeper!" << endl;

    KeeperApplication& instance = KeeperApplication::getInstance();
    instance.loadDatabasePlugins("plugins_conf.json");
    return instance.execute();
}