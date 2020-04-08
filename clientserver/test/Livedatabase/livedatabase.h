#ifndef LIVEDATABASE_H
#define LIVEDATABASE_H

#include <vector>
#include <string>
#include "newsgroup.h"
#include "article.h"

using std::vector;
using std::string;

class LiveDataBase {
public:
  LiveDataBase() = default;
  void addNewsgroup(string);
  void removeNewsgroup(long long int GroupId);
  void addArtikel(string, string, string, long long int);
  void removeArtikel(long long int, long long int);
  vector<Article> listArtikels(long long int); 
  vector<Newsgroup> listNewsgroups();
private:
  vector<Newsgroup> StoreNewsG; 
  long long int idNextNewsG{};
};

#endif
