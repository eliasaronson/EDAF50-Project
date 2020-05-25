#include "connection.h"
#include "casehandler.h"
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
    if (argc != 3) {
        cerr << "Usage: myserver port-number database-type" << endl << "Database-type: 0 for live or 1 for disk." << endl << "Example: ./myserver 1050 0" << endl;
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

void runServer(Server& server, unique_ptr<DataBase> &db) {
    while (true) {
        cout << endl;
        cout << "Waiting for server activity." << endl;
        auto conn = server.waitForActivity();
        cout << "Server active." << endl;


        if (conn != nullptr) {

            MessageHandler mess(*conn.get());
            try {
                //Handle message
                cout << "Sending message to case handler." << endl;
                CaseHandler::CaseHandler(mess, *db);
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
}

unique_ptr<DataBase> chooseDatabase(int ops) {
    cout << ops << endl;
    if (ops == 0) {
        cout << "Creating live database." << endl;
        unique_ptr<DataBase> db (new LiveDataBase());
        return db;
    } else {
        // Create diskdatabase instead
        cout << "Creating disk database." << endl;
        unique_ptr<DataBase> db (new DiskDataBase()); 
        return db;
    }
}

int main(int argc, char* argv[]) {
    cout << "Starting server." << endl;

    auto server = init(argc, argv);
    auto db = chooseDatabase(stoi(argv[2]));
    runServer(server, db);

    return 0;
}
