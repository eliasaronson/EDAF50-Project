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
#include <regex>

using std::string;
using std::istream;

class DiskDataBase : public DataBase{
public:
  DiskDataBase();
  DiskDataBase(const DiskDataBase& d) = default;
  ~DiskDataBase() = default;
  void addNewsgroup(string name) override;
  void removeNewsgroup(int groupId) override;
  void addArtikel(string title, string auth, string text, int grpId) override;
  void removeArtikel(int groupId, int artId) override;
  vector<Article> listArtikels(int id) override; 
  vector<Newsgroup> listNewsgroups() override;
  Article getArtikel(int grpId, int artId) override;
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
