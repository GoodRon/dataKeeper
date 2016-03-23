/*
 * Created by Roman Meyta 2016 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <memory>
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx"
#include "message.hxx"
#include "message_odb.h"

using namespace std;
using namespace odb::core;

int main(int argc, char** argv) {
    try {
        auto_ptr<database> db(create_database(argc, argv));

        {
            Message msg(1, "test", 1, 101, 17, 0, 0, false, 0, "channel1");

            transaction t(db->begin());

            auto msgId = db->persist(msg);

            t.commit();
        }

//        typedef odb::query<Message> query;
//        typedef odb::result<Message> result;
//
//        {
//            transaction t (db->begin ());
//
//            result r (db->query<Message> (query::getMid > 0));
//
//            for (result::iterator i (r.begin ()); i != r.end (); ++i)
//            {
//                cout << "Hello, " i->getMid() << "!" << endl;
//            }
//
//            t.commit ();
//        }

    } catch (const odb::exception& e) {
        cerr << e.what () << endl;
        return -1;
    }
    return 0;
}
