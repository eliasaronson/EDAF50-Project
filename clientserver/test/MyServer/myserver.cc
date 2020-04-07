/* myserver.cc: sample server program */
#include "connection.h"
#include "connectionclosedexception.h"
#include "server.h"
#include "protocol.h"
#include "messagehandler.h"
#include "livedatabase.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace std;



Server init(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: myserver port-number" << endl;
        exit(1);
    }
    int port = -1;
    try {
        port = stoi(argv[1]);
    } catch (exception& e) {
        cerr << "Wrong format for port number. " << e.what() << endl;
        exit(2);
    }
    Server server(port);
    if (!server.isReady()) {
        cerr << "Server initialization error." << endl;
        exit(3);
    }
    return server;
}

void ListNGs(MessageHandler& mess, LiveDataBase& db){

    mess.writeInt(Protocol::ANS_LIST_NG);

    auto tmp = db.listNewsgroups();  
    mess.writeInt(tmp.size());

    for (auto x : tmp) {
      string res = to_string(x.getId()) + " " + x.getName();
      mess.writeString(res);
    }
    
    mess.writeInt(Protocol::ANS_END);
}

void createNG(MessageHandler& mess, LiveDataBase& db){

    mess.writeInt(Protocol::ANS_CREATE_NG);
    string name = mess.readString();

    string res = db.addNewsgroup(name);
    if (res.empty()) {
        mess.writeInt(Protocol::ANS_NAK);
        mess.writeInt(Protocol::ERR_NG_ALREADY_EXISTS);
    }else {
        mess.writeInt(Protocol::ANS_ACK);
    }

    mess.writeInt(Protocol::ANS_END);
}

void deleteNG(MessageHandler& mess, LiveDataBase& db){

    mess.writeInt(Protocol::ANS_DELETE_NG);
    long long int id = mess.readNumber();

    string res = db.removeNewsgroup(id);
    if (res.empty()) {
        mess.writeInt(Protocol::ANS_NAK);
        mess.writeInt(Protocol::ERR_NG_DOES_NOT_EXIST);
    }else {
        mess.writeInt(Protocol::ANS_ACK);
    }

    mess.writeInt(Protocol::ANS_END);
}

void listA(MessageHandler& mess, LiveDataBase& db){

    mess.writeInt(Protocol::ANS_LIST_NG);
    long long int id = mess.readNumber();

    auto tmp = db.listArtikels(id);  
    // Lägg in check för error
    
    //If works
    mess.writeInt(Protocol::ANS_ACK);
    mess.writeInt(tmp.size());
    for (auto x : tmp) {
      string res = to_string(x.getId()) + " " + x.getTitle();
      mess.writeString(res);
    }

    //If not
    //
    //
    
    
    
    mess.writeInt(Protocol::ANS_END);
}

string case_handler(MessageHandler& mess, LiveDataBase& db) {
    string resualt;
    switch (mess.usrCommand())
    {
      case Protocol::COM_LIST_NG: ListNGs(mess, db);
        break;
      case Protocol::COM_CREATE_NG: createNG(mess, db);
        break;
      case Protocol::COM_DELETE_NG: deleteNG(mess, db);
        break;
      case Protocol::COM_LIST_ART: listA(mess, db);
        break;
      default:
        break;
    }
    return resualt;
}

int main(int argc, char* argv[]) {

    auto server = init(argc, argv);

    if (stoi(argv[2]) == 1) {
        LiveDataBase datab{}; // Is that correct initiation
    } else {
        // Create diskdatabase with same name
    }

    while (true) {
        auto conn = server.waitForActivity();

        MessageHandler mess(conn);

        if (conn != nullptr) {
            try {

                case_handler(mess, datab);

            } catch (ConnectionClosedException&) {
                server.deregisterConnection(conn);
                cout << "Client closed connection" << endl;
            }
        } else {
            conn = make_shared<Connection>();
            server.registerConnection(conn);
            cout << "New client connects" << endl;
        }
    }
    return 0;
}
