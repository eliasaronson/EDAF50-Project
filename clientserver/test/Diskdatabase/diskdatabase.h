#ifndef DISKDATABASE_H
#define DISKEDATABASE_H

#include <vector>
#include <string>
#include "newsgroup.h"
#include "article.h"

using std::vector;
using std::string;

class DiskDataBase {
public:
  DiskDataBase();
  void addNewsgroup(string);
  void removeNewsgroup(long long int GroupId);
  void addArtikel(Article, long long int);
  void removeArtikel(long long int, long long int);
  vector<Article> listArtikels(long long int); 
  vector<Newsgroup> listNewsgroups();
private:
  vector<Newsgroup> StoreNewsG; 
  long long int idNextNewsG{};
};

#endif
