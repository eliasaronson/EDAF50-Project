#include "connection.h"
#include "messagehandler.h"
#include "connectionclosedexception.h"
#include "server.h"
#include "protocol.h"
#include "livedatabase.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace std;

//MessageHandler::MessageHandler(const shared_ptr<Connection>& con) : conn(con) {}
MessageHandler::MessageHandler(Connection& con) : conn(con) {}
/*
* Read an integer from a client.
*/
int MessageHandler::readNumber() {
    unsigned char byte1 = conn.read();
    unsigned char byte2 = conn.read();
    unsigned char byte3 = conn.read();
    unsigned char byte4 = conn.read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

string MessageHandler::readString() {
    string res{};
    char c;

    while((c = conn.read()) != '$') {
        res += c;
    }


    return res;
}

//Reads parameters according to messaging protocol.
//string_p: PAR_STRING N char1 char2 ... charN  // N is the number of characters
//num_p: PAR_NUM N                              // N is the number
//
//Maybe change return type to template class??
string MessageHandler::readParam() {

    string res{};
    Protocol IorS = usrCommand();
    if(IorS == Protocol::PAR_STRING) {
        int N = readNumber();
        char c;

        for(int i = 0; i < N; i++) {
            c = conn.read();
            res += c;
        }


    } else if(IorS == Protocol::PAR_NUM) {
        res = to_string(readNumber());
    } else {
        string err = "Error in message: " + to_string(static_cast<int>(IorS)) + " when a int-41 or string-40 parameter was expected!";
        throw runtime_error(err);
    }

    return res;
}

Protocol MessageHandler::usrCommand() {
    char c = conn.read();
    return static_cast<Protocol>(c);
}
/*
* Send a string to a client.
*/
void MessageHandler::writeString(const string& s) {
    int n = s.size();
    writeInt(Protocol::PAR_STRING);
    directIntWriter(n);

    for (char c : s) {
        conn.write(c);
    }
}

void MessageHandler::writeInt(const int& i) {
    writeInt(Protocol::PAR_NUM);
    directIntWriter(i);
}

void MessageHandler::directIntWriter(const int& i) {
    conn.write((i >> 24) & 0xFF);
    conn.write((i >> 16) & 0xFF);
    conn.write((i >> 8) & 0xFF);
    conn.write((i) & 0xFF);
}

void MessageHandler::writeInt(const Protocol& p) {
    conn.write(static_cast<unsigned char>(p));
}

void MessageHandler::comEnd() {
    auto com = usrCommand();
    if(com != (Protocol::COM_END)) {
        writeInt(Protocol::COM_END);
        throw runtime_error("Error in message. End of message not recived when expected!");
    }
}
