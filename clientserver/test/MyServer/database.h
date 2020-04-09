#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include "newsgroup.h"
#include "article.h"

using std::vector;
using std::string;

class DataBase {
public:
  virtual ~DataBase() = default;
  virtual void addNewsgroup(string);
  virtual void removeNewsgroup(long long int);
  virtual void addArtikel(string, string, string, long long int);
  virtual void removeArtikel(long long int, long long int);
  virtual vector<Article> listArtikels(long long int); 
  virtual vector<Newsgroup> listNewsgroups();
  virtual Article getArtikel(long long int, long long int);
};

#endif
