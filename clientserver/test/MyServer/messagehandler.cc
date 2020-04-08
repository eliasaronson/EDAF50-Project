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

MessageHandler::MessageHandler(const shared_ptr<Connection>& con) : conn(con){}
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

string MessageHandler::readString(){
    string res{};  

    char c = conn->read();
    while(c != '$'){
      res += c;
    } 

    return res;
}

Protocol MessageHandler::usrCommand(){
  unsigned char byte1 = conn->read();
  unsigned char byte2 = conn->read();
  unsigned char byte3 = conn->read();
  unsigned char byte4 = conn->read();
  int tmp =  (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
  return static_cast<Protocol>(tmp);
}
/*
* Send a string to a client.
*/
void MessageHandler::writeString(const string& s) {
    for (char c : s) {
        conn->write(c);
    }
    conn->write('$');
}

void MessageHandler::writeInt(const int& i){
  conn->write((i >> 24) & 0xFF);
  conn->write((i >> 16) & 0xFF);
  conn->write((i >> 8) & 0xFF);
  conn->write((i) & 0xFF);
}

void MessageHandler::writeInt(const Protocol& p){
  writeInt(static_cast<int>(p));
}

void MessageHandler::comEnd(){
  int com = readNumber();  
  if(com != static_cast<int>(Protocol::COM_END)){
    throw exception();
  }
}
