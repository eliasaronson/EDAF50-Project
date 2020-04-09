/* myserver.cc: sample server program */
#include "connection.h"
#include "database.h"
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

void ListNGs(MessageHandler& mess, DataBase& db){
    //Input
    mess.comEnd();
    //Output
    mess.writeInt(Protocol::ANS_LIST_NG);

    auto tmp = db.listNewsgroups();  
    mess.writeInt(tmp.size());

    for (auto x : tmp) {
      string res = to_string(x.getId()) + " " + x.getName();
      mess.writeString(res);
    }
    
    mess.writeInt(Protocol::ANS_END);
}

void createNG(MessageHandler& mess, DataBase& db){

    //Input
    string name = mess.readString();
    mess.comEnd();

    //Output
    mess.writeInt(Protocol::ANS_CREATE_NG);

    try{
        db.addNewsgroup(name);
    } catch (exception& e) {
        mess.writeInt(Protocol::ANS_NAK);
        mess.writeInt(Protocol::ERR_NG_ALREADY_EXISTS);
        mess.writeInt(Protocol::ANS_END);
        return;
    }

    mess.writeInt(Protocol::ANS_ACK);
    mess.writeInt(Protocol::ANS_END);
}

void deleteNG(MessageHandler& mess, DataBase& db){
    //Input
    long long int id = mess.readNumber();
    mess.comEnd();
    //Output
    mess.writeInt(Protocol::ANS_DELETE_NG);

    try{
        db.removeNewsgroup(id);
    } catch (exception& e) {
        mess.writeInt(Protocol::ANS_NAK);
        mess.writeInt(Protocol::ERR_NG_DOES_NOT_EXIST);
        mess.writeInt(Protocol::ANS_END);
        return;
    }

    mess.writeInt(Protocol::ANS_ACK);
    mess.writeInt(Protocol::ANS_END);
}

void listA(MessageHandler& mess, DataBase& db){
    //Input
    long long int id = mess.readNumber();
    mess.comEnd();
    //Output
    mess.writeInt(Protocol::ANS_LIST_ART);

    // Lägg in check för error
    
    //If works
    try{
        auto tmp = db.listArtikels(id);  
    } catch (exception& e) {
        mess.writeInt(Protocol::ANS_NAK);
        mess.writeInt(Protocol::ERR_NG_DOES_NOT_EXIST);
        mess.writeInt(Protocol::ANS_END);
        return;
    }
    
    auto tmp = db.listArtikels(id); //Quick fix. Come up with better solution. 

    mess.writeInt(Protocol::ANS_ACK);
    mess.writeInt(tmp.size());
    for (auto x : tmp) {
      string res = to_string(x.getId()) + " " + x.getTitle();
      mess.writeString(res);
    }
    
    mess.writeInt(Protocol::ANS_END);
}

void createA(MessageHandler& mess, DataBase& db){
    //Input
    long long int id = mess.readNumber();
    string title = mess.readString();
    string auth = mess.readString();
    string text = mess.readString();
    mess.comEnd();
    //Output
    mess.writeInt(Protocol::ANS_CREATE_ART);

    try{
        db.addArtikel(title, auth, text, id); 
    } catch (exception& e) {
        mess.writeInt(Protocol::ANS_NAK);
        mess.writeInt(Protocol::ERR_NG_DOES_NOT_EXIST);
        mess.writeInt(Protocol::ANS_END);
        return;
    }
    
    mess.writeInt(Protocol::ANS_ACK);
    
    mess.writeInt(Protocol::ANS_END);
}

void deleteA(MessageHandler& mess, DataBase& db){
    //Input
    long long int idG = mess.readNumber();
    long long int idA = mess.readNumber();
    mess.comEnd();
    //Output
    mess.writeInt(Protocol::ANS_DELETE_ART);

    try{
        db.removeArtikel(idG, idA);
    } catch (exception& e) {
        mess.writeInt(Protocol::ANS_NAK);
        if(e.what() == to_string(static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST))){
            mess.writeInt(Protocol::ERR_NG_DOES_NOT_EXIST);
        } else{
            mess.writeInt(Protocol::ERR_ART_DOES_NOT_EXIST);
        }
        mess.writeInt(Protocol::ANS_END);
        return;
    }
    
    mess.writeInt(Protocol::ANS_ACK);
            
    mess.writeInt(Protocol::ANS_END);
}

void getA(MessageHandler& mess, DataBase& db){
    //Input
    long long int idG = mess.readNumber();
    long long int idA = mess.readNumber();
    mess.comEnd();
    //Output
    mess.writeInt(Protocol::ANS_GET_ART);


    try{
        Article a = db.getArtikel(idG, idA);
    } catch (exception& e) {
        mess.writeInt(Protocol::ANS_NAK);
        if(e.what() == to_string(static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST))){
            mess.writeInt(Protocol::ERR_NG_DOES_NOT_EXIST);
        } else{
            mess.writeInt(Protocol::ERR_ART_DOES_NOT_EXIST);
        }
        mess.writeInt(Protocol::ANS_END);
        return;
    }

    Article a = db.getArtikel(idG, idA); //Again quick fix

    mess.writeInt(Protocol::ANS_ACK);
    mess.writeString(a.getTitle());
    mess.writeString(a.getAuthor());
    mess.writeString(a.getText());
            
    mess.writeInt(Protocol::ANS_END);
}

void case_handler(MessageHandler& mess, DataBase& db) {
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
      case Protocol::COM_CREATE_ART: createA(mess, db);
        break;
      case Protocol::COM_DELETE_ART: deleteA(mess, db);
        break;
      case Protocol::COM_GET_ART: getA(mess, db);
        break;
      default: //Maybe some error
        break;
    }
}

int main(int argc, char* argv[]) {

    auto server = init(argc, argv);
    DataBase *db;
    if (stoi(argv[2]) == 1) {
        db = new LiveDataBase();
    } else {
        // Create diskdatabase instead
        db = new LiveDataBase();
    }
    
    

    while (true) {
        auto conn = server.waitForActivity();

        MessageHandler mess(conn);

        if (conn != nullptr) {
            try {
                //Handle message
                case_handler(mess, *db);
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
    delete(db);
    return 0;
}