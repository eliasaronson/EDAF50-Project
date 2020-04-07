#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "connection.h"
#include <string>

using std::string;

class MessageHandler {
public:
  MessageHandler(Connection&);
private:
  Connection& conn;
};

#endif
