#include "connection.h"
#include "database.h"
#include "connectionclosedexception.h"
#include "server.h"
#include "protocol.h"
#include "messagehandler.h"
#include "livedatabase.h"
#include "diskdatabase.h"
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
    cout << "Listing newsgroup command recived." << endl;
    //Input
    mess.comEnd();
    //Output
    mess.writeInt(Protocol::ANS_LIST_NG);
    cout << "Awnserd command." << endl;

    auto tmp = db.listNewsgroups();  
    cout << "Newsgoups recived from server. Number of NGs: " << tmp.size() << endl;
    mess.writeInt(tmp.size());

    for (auto x : tmp) {
      cout << "Sending newsgroups." << endl;
      int id = x.getId();
      string name = x.getName();
      cout << id << " " << name << endl;

      mess.writeInt(id);
      mess.writeString(name);
    }
    
    mess.writeInt(Protocol::ANS_END);
    cout << "Awnsered list newsgroups. Returing to standby." << endl;
}

void createNG(MessageHandler& mess, DataBase& db){

    cout << "Creating Newsgroup." << endl;
    
    //Input
    string name = mess.readParam();
    cout << "Input recived." << endl;
    mess.comEnd();

    //Output
    mess.writeInt(Protocol::ANS_CREATE_NG);

    cout << "Anwsering command." << endl;

    try{
        cout << "Creating Newsgroup in database." << endl;
        db.addNewsgroup(name);
    } catch (int& e) {
        cout << "Error occorded. Sending error messsage to client." << endl;
        mess.writeInt(Protocol::ANS_NAK);
        mess.writeInt(static_cast<Protocol>(e));
        mess.writeInt(Protocol::ANS_END);
        return;
    }
    catch (exception& e)
    {
      mess.writeInt(Protocol::ANS_NAK);
      mess.writeInt(Protocol::ANS_END);
      return;
    }

    cout << "Newsgroup created." << endl;
    mess.writeInt(Protocol::ANS_ACK);
    mess.writeInt(Protocol::ANS_END);
    cout << "Anwser sent. Waiting for new command." << endl;
}

void deleteNG(MessageHandler& mess, DataBase& db){
    cout << "Delete newsgroup called." << endl;
    //Input
    int id = stoi(mess.readParam());
    cout << "Recived Id: " << id << endl;
    mess.comEnd();
    //Output
    mess.writeInt(Protocol::ANS_DELETE_NG);

    try{
        cout << "Trying to remove newsgroup." << endl;
        db.removeNewsgroup(id);
    } catch (int& e) {
        cout << "No newsgroup deleted. Sending error code." << endl;
        mess.writeInt(Protocol::ANS_NAK);
        mess.writeInt(static_cast<Protocol>(e));
        mess.writeInt(Protocol::ANS_END);
        return;
    } catch (exception& e) {
      mess.writeInt(Protocol::ANS_NAK);
      mess.writeInt(Protocol::ANS_END);
      return;
    }

    cout << "Delete done." << endl;
    mess.writeInt(Protocol::ANS_ACK);
    mess.writeInt(Protocol::ANS_END);
}

void listA(MessageHandler& mess, DataBase& db){
    cout << "List articles called." << endl;
    //Input
    int id = stoi(mess.readParam());
    mess.comEnd();
    cout << "Recived Id: " << id << endl;
    //Output
    mess.writeInt(Protocol::ANS_LIST_ART);

    // Lägg in check för error
    
    //If works
    try{
        cout << "Trying to get artikles." << endl;
        auto tmp = db.listArtikels(id);  
    } catch (int& e) {
        cout << "No, artikels recived. Sending error code to client." << endl;
        mess.writeInt(Protocol::ANS_NAK);
        mess.writeInt(static_cast<Protocol>(e));
        mess.writeInt(Protocol::ANS_END);
        return;
    }
    catch (exception& e)
    {
      cout << "Unexpected error occured!" << endl;
      mess.writeInt(Protocol::ANS_NAK);
      mess.writeInt(Protocol::ANS_END);
      return;
    }
    
    auto tmp = db.listArtikels(id); //Quick fix. Come up with better solution. 

    cout << "Artkales recived. Sending..." << endl;
    mess.writeInt(Protocol::ANS_ACK);
    mess.writeInt(tmp.size());
    for (auto x : tmp) {
      int id = x.getId();
      string title = x.getTitle();
      cout << "Sending: " << id << " " << title << endl;
      mess.writeInt(id);
      mess.writeString(title);
    }
    
    mess.writeInt(Protocol::ANS_END);
}

void createA(MessageHandler& mess, DataBase& db){
    cout << "Creating article." << endl;
    //Input
    int id = stoi(mess.readParam());
    string title = mess.readParam();
    string auth = mess.readParam();
    string text = mess.readParam();
    cout << "Input recvied." << endl;
    mess.comEnd();
    cout << "End of command recvied." << endl;
    //Output
    mess.writeInt(Protocol::ANS_CREATE_ART);

    cout << "Ansering command." << endl;
    try{
        db.addArtikel(title, auth, text, id); 
        cout << "Testing, create article." << endl;
    } catch (int& e) {
        cout << "Could not create artikle." << endl;
        mess.writeInt(Protocol::ANS_NAK);
        mess.writeInt(static_cast<Protocol>(e));
        mess.writeInt(Protocol::ANS_END);
        cout << "Sending end of command." << endl;
        return;
    }
    catch (exception& e)
    {
      mess.writeInt(Protocol::ANS_NAK);
      mess.writeInt(Protocol::ANS_END);
      return;
    }
    
    cout << "Article created." << endl;
    mess.writeInt(Protocol::ANS_ACK);
    
    mess.writeInt(Protocol::ANS_END);
    cout << "Sending end of command." << endl;
}

void deleteA(MessageHandler& mess, DataBase& db){
    cout << "Delete artikle called." << endl;
    //Input
    int idG = stoi(mess.readParam());
    int idA = stoi(mess.readParam());
    cout << "Inputs recived: " << idG << " " << idA << endl;
    mess.comEnd();
    //Output
    mess.writeInt(Protocol::ANS_DELETE_ART);

    try{
        cout << "Trying to remove article." << endl;
        db.removeArtikel(idG, idA);
    } catch (int& e) {
        cout << "Remove failed. Sending error code." << endl;
        mess.writeInt(Protocol::ANS_NAK);
        mess.writeInt(static_cast<Protocol>(e));
        mess.writeInt(Protocol::ANS_END);
        return;
    }
    catch (exception& e)
    {
      cout << "Remove failed for unkown reason." << endl;
      mess.writeInt(Protocol::ANS_NAK);
      mess.writeInt(Protocol::ANS_END);
      return;
    }
    
    mess.writeInt(Protocol::ANS_ACK);
    cout << "Remove succesfull." << endl;
            
    mess.writeInt(Protocol::ANS_END);
}

void getA(MessageHandler& mess, DataBase& db){
    //Input
    int idG = stoi(mess.readParam());
    int idA = stoi(mess.readParam());
    mess.comEnd();
    //Output
    mess.writeInt(Protocol::ANS_GET_ART);


    try{
        Article a = db.getArtikel(idG, idA);
    } catch (int& e) {
        mess.writeInt(Protocol::ANS_NAK);
        mess.writeInt(static_cast<Protocol>(e));
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
    cout << "Case handler called." << endl;
    auto m = mess.usrCommand();
    cout << "Message recived:" << static_cast<int>(m) << endl;
    switch (m)
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
    cout << "Startring server." << endl;

    auto server = init(argc, argv);
    DataBase *db;
    if (true) {
        cout << "Creating live database." << endl;
        db = new LiveDataBase();
    } else {
        // Create diskdatabase instead
        cout << "Creating disk database." << endl;
        db = new LiveDataBase();
    }
    
    

    while (true) {
        cout << endl;
        cout << "Waiting for server activity." << endl;
        auto conn = server.waitForActivity();
        cout << "Server active." << endl;

        MessageHandler mess(conn);

        if (conn != nullptr) {
            try {
                //Handle message
                cout << "Sending message to case handler." << endl;
                case_handler(mess, *db);
            } catch (ConnectionClosedException&) {
                server.deregisterConnection(conn);
                cout << "Client closed connection" << endl;
            } catch (exception& e) {
              cout << "\033[1;31mError: \033[0m";
              cout << e.what() << endl;
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
