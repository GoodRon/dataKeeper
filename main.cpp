/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <iostream>
#include <unistd.h>

#include "KeeperApplication.h"

using namespace std;

int main(int argc, char** argv) {
    int rez = 0;
    const string defaultConfig("/etc/dataKeeper.conf");
    string config(defaultConfig);
    while ( (rez = getopt(argc,argv,"c:h")) != -1 ) {
        switch (rez) {
            case 'c':
                config = optarg;
                break;
            case 'h':
                cout << "dataKeeper - plugin based database manager" << endl;
                cout << "Usage: dataKeeper [options]" << endl;
                cout << "If no config path specified, default " << defaultConfig
                     << " will be used" << endl;
                cout << "-c <path_to_config> - path to configuration file" << endl;
                cout << "-h - print this help" << endl;
                return 0;
            default:
                break;
        }
    }

    KeeperApplication& instance = KeeperApplication::getInstance();
    instance.loadDatabasePlugins(config);
    return instance.execute();
}