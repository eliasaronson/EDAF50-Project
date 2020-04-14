#ifndef DISKDATABASE_H
#define DISKEDATABASE_H

#include <string>
#include <sys/stat.h>
#include <iostream>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include <pwd.h>

using std::string;
using std::istream;

class DiskDataBase {
public:
  DiskDataBase() = default;
  DiskDataBase(const DiskDataBase& d) = default;
  void addNewsgroup(const string& name);
  void removeNewsgroup(size_t& groupId);
  void addArticle(string& title, string& auth, string& text, size_t& grpId);
  void removeArticle(size_t& groupId, size_t& artId);
  //vector<Article> listArtikels(size_t&); 
  //vector<Newsgroup> listNewsgroups();
private:
  size_t idNextNewsG = 0;
};

bool containsWord(const string& name);
int currentIndex();
string getName(size_t& groupId);
string getId_txt();
int nextArtId(size_t& grpId);
bool containsArticle(size_t& grpId, size_t& artId);

#endif
