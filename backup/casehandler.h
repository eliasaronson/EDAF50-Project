#ifndef CASEHANDLER_H
#define CASEHANDLER_H

#include <string>
#include "messagehandler.h"
#include "database.h"

using std::string;

class CaseHandler {
public:
  CaseHandler(MessageHandler&, DataBase&);

private:
  void getA(MessageHandler&, DataBase&);
  void deleteA(MessageHandler&, DataBase&);
  void createA(MessageHandler&, DataBase&);
  void listA(MessageHandler&, DataBase&);
  void deleteNG(MessageHandler&, DataBase&);
  void createNG(MessageHandler&, DataBase&);
  void ListNGs(MessageHandler&, DataBase&);
};

#endif
