/* myserver.cc: sample server program */
#include "connection.h"
#include "connectionclosedexception.h"
#include "server.h"
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

string message_handler(int& nbr) {
    string resualt;
    switch (nbr)
    {
    case 1: result = 
        break;
    case 2: // code to be executed if n = 2;
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

        if (conn != nullptr) {
            try {
                int nbr = readNumber(conn);

                string resualt = message_handler(nbr);

                writeString(conn, result);
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
