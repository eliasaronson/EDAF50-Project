#ifndef DISKDATABASE_H
#define DISKDATABASE_H

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
  void addNewsgroup(string) override;
  void removeNewsgroup(int) override;
  void addArtikel(string, string, string, int) override;
  void removeArtikel(int, int) override;
  vector<Article> listArtikels(int) override;
  vector<Newsgroup> listNewsgroups() override;
  Article getArtikel(int, int) override;
private:
};

//bool containsWord(string& name);
int currentIndex();
string getName(int& groupId);
string getId_txt();
int nextArtId(int& grpId);
Article* containsArticle(int& grpId, int& artId);
string getDbDir();

#endif
