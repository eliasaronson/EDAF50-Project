/**@file	/home/olofk/EDAF50-Project/clientserver/memServer.h
 * @author	olofk
 * @version	802
 * @date
 * 	Created:	25th Mar 2020
 * 	Last Update:	25th Mar 2020
 */

#ifndef MAINSERVER_H
#define MAINSERVER_H

#include "server.h"

using std::vector;
using std::string;

class MemServer {
public:
  /* Creates a server that stores in the primary memory*/
  MemServer();
private:
  bool database;
  vector<string> newsgroups; 
};

#endif  // MAINSERVER_H
