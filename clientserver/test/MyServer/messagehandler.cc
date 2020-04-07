#include "connection.h"
#include "messagehandler.h"
#include "connectionclosedexception.h"
#include "server.h"
#include "livedatabase.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace std;

MessageHandler::MessageHandler(Connection& con){
  conn(con);
}
/*
* Read an integer from a client.
*/
int readNumber(const shared_ptr<Connection>& conn) {
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

/*
* Send a string to a client.
*/
void writeString(const shared_ptr<Connection>& conn, const string& s) {
    for (char c : s) {
        conn->write(c);
    }
    conn->write('$');
}
