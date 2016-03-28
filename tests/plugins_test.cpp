/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <memory>
#include <iostream>
#include <dlfcn.h>
#include <unistd.h>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "message.hxx"
#include "message_odb.h"

using namespace std;
using namespace odb::core;

typedef database*(*pluginIface)(int, char**);

int main(int argc, char** argv) {
//    if (argc < 2) {
//        cout << "database plugin is needed!" << endl;
//        return 1;
//    }

//    auto handle = dlopen(argv[1], RTLD_LAZY);
    auto handle = dlopen("./libdbplugin.so", RTLD_LAZY);
    if (!handle) {
        cout << "can't open plugin! Error: " << dlerror() << endl;
        return 1;
    }

    pluginIface func;
    func = reinterpret_cast<pluginIface>(dlsym(handle, "returnDatabase"));
    if (!func) {
        cout << "can't load symbol! Error: " << dlerror() << endl;
        return 1;
    }

    try {
        unique_ptr<database> db((*func)(argc, argv));

        typedef odb::query<Message> query;
        typedef odb::result<Message> result;

        {
            Message msg(1, "TTTTest", 1, 101, 17, 0, 0, false, 0, "channel2");

            transaction t(db->begin());

            auto msgId = db->persist(msg);

            t.commit();
        }

        {
            transaction t(db->begin());

            result r(db->query<Message> (query::mid > 0));

            for (result::iterator i (r.begin ()); i != r.end (); ++i) {
                cout << "Hello, " << i->getMid() << " " << i->getStatus() << "!" << endl;
            }

            t.commit ();
        }
    } catch (const odb::exception& e) {
        cerr << e.what () << endl;
        return -1;
    }

    dlclose(handle);
    return 0;
}
