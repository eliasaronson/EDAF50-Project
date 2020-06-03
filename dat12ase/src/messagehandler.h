#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "connection.h"
#include "connectionclosedexception.h"
#include "server.h"
#include "protocol.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include "connection.h"

using std::string;
using std::shared_ptr;

class MessageHandler {
public:
//  MessageHandler(const shared_ptr<Connection>&);
  MessageHandler(Connection& con);
  int readNumber();
  string readString();
  string readParam();
  Protocol usrCommand();
  void writeString(const string&);
  void writeInt(const int&);
  void writeInt(const Protocol&);
  void directIntWriter(const int&);
  void comEnd();
private:
//  const shared_ptr<Connection>& conn;
  Connection& conn;
};

#endif
