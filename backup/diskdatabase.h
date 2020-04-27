#ifndef DISKDATABASE_H
#define DISKEDATABASE_H

#include "database.h"
#include "article.h"
#include "newsgroup.h" 
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
#include <vector>

using std::string;
using std::istream;

class DiskDataBase : public DataBase{
public:
  DiskDataBase() = default;
  DiskDataBase(const DiskDataBase& d) = default;
  ~DiskDataBase() = default;
  void addNewsgroup(const string& name);
  void removeNewsgroup(int& groupId);
  void addArticle(string& title, string& auth, string& text, int& grpId);
  void removeArticle(int& groupId, int& artId);
  vector<Article> listArtikels(const int& id); 
  vector<Newsgroup> listNewsgroups();
  Article getArtikel(int& grpId, int& artId);
private:
};

bool containsWord(const string& name);
int currentIndex();
string getName(int& groupId);
string getId_txt();
int nextArtId(int& grpId);
Article* containsArticle(int& grpId, int& artId);
string getDbDir();

#endif
