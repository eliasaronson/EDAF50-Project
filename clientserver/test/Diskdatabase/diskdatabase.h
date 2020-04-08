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
#include <sstream>

using std::string;
using std::istream;

class DiskDataBase {
public:
  DiskDataBase() = default;
  DiskDataBase(const DiskDataBase& d) = default;
  void addNewsgroup(const string& name);
  void removeNewsgroup(size_t& groupId);
  void addArticle(string& article, size_t& artId);
  void removeArticle(size_t& groupId, size_t& artId);
  //vector<Article> listArtikels(size_t&); 
  //vector<Newsgroup> listNewsgroups();
private:
  size_t idNextNewsG = 0;
};

bool containsWord(const string& name);
int currentIndex();

#endif
