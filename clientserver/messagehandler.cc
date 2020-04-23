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

MessageHandler::MessageHandler(const shared_ptr<Connection>& con) : conn(con) {}
/*
* Read an integer from a client.
*/
int MessageHandler::readNumber() {
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

string MessageHandler::readString() {
    cout << "Reading string." << endl;
    string res{};
    char c;

    cout << "Information recived." << endl;
    while((c = conn->read()) != '$') {
        cout << "Char: " << c << endl;
        res += c;
    }

    cout << "Information translated to string: " << res << endl;

    return res;
}

//Reads parameters according to messaging protocol.
//string_p: PAR_STRING N char1 char2 ... charN  // N is the number of characters
//num_p: PAR_NUM N                              // N is the number
//
//Maybe change return type to template class??
string MessageHandler::readParam() {
    cout << "Reading string." << endl;

    string res{};
    Protocol IorS = usrCommand();
    if(IorS == Protocol::PAR_STRING) {
        cout << "String input recived." << endl;
        int N = readNumber();
        cout << "Numbers of characters in input: " << N << endl;
        char c;

        cout << "Information recived." << endl;
        for(int i = 0; i < N; i++) {
            c = conn->read();
            cout << "Char: " << c << endl;
            res += c;
        }

        cout << "Information translated to string: " << res << endl;

    } else if(IorS == Protocol::PAR_NUM) {
        cout << "Int input recived." << res << endl;
        res = to_string(readNumber());
    } else {
        string err = "Error in message: " + to_string(static_cast<int>(IorS)) + " when a int-41 or string-40 parameter was expected!";
        throw runtime_error(err);
    }

    return res;
}

Protocol MessageHandler::usrCommand() {
    cout << "Reading user command." << endl;
    char c = conn->read();
    cout << "Recived: " << static_cast<int>(c) << endl;
    return static_cast<Protocol>(c);
}
/*
* Send a string to a client.
*/
void MessageHandler::writeString(const string& s) {
    int n = s.size();
    cout << "String length: " << n << endl;
    writeInt(Protocol::PAR_STRING);
    directIntWriter(n);

    for (char c : s) {
        conn->write(c);
    }
}

void MessageHandler::writeInt(const int& i) {
    writeInt(Protocol::PAR_NUM);
    directIntWriter(i);
}

void MessageHandler::directIntWriter(const int& i) {
    conn->write((i >> 24) & 0xFF);
    conn->write((i >> 16) & 0xFF);
    conn->write((i >> 8) & 0xFF);
    conn->write((i) & 0xFF);
}

void MessageHandler::writeInt(const Protocol& p) {
    conn->write(static_cast<unsigned char>(p));
}

void MessageHandler::comEnd() {
    cout << "Reading message for comEnd." << endl;
    auto com = usrCommand();
    cout << "Checking for end of message." << endl;
    if(com != (Protocol::COM_END)) {
        writeInt(Protocol::COM_END);
        throw runtime_error("Error in message. End of message not recived when expected!");
    }
}
