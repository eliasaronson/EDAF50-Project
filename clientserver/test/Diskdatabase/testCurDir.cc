/**@file	/home/olofk/EDAF50-Project/clientserver/test/Diskdatabase/testCurDir.cc
 * @author	olofk
 * @version	802
 * @date
 * 	Created:	17th Apr 2020
 * 	Last Update:	17th Apr 2020
 */
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <cstring>

using std::string;

string getCurrentWorkingDir(){
  char path[200];
  getcwd(path,200);
  char* pch = path;
  pch[strlen(pch)-4]=0;
  string ret(path);
  return ret;
}

int main(){
  std::cout << (getCurrentWorkingDir()) << std::endl;
  return 1;
}
