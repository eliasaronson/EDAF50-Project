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
#include <../MyServer/article.h>
#include <../MyServer/newsgroup.h>
#include <vector>

using std::string;
using std::istream;

class DiskDataBase {
public:
  DiskDataBase() = default;
  DiskDataBase(const DiskDataBase& d) = default;
  void addNewsgroup(const string& name);
  void removeNewsgroup(int& groupId);
  void addArticle(string& title, string& auth, string& text, int& grpId);
  void removeArticle(int& groupId, int& artId);
  vector<Article> listArtikels(const long long int& id); 
  vector<Newsgroup> listNewsgroups();
private:
};

bool containsWord(const string& name);
int currentIndex();
string getName(int& groupId);
string getId_txt();
int nextArtId(int& grpId);
bool containsArticle(int& grpId, int& artId);

#endif
