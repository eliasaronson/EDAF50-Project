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
  virtual void addNewsgroup(string) = 0;
  virtual void removeNewsgroup(int) = 0;
  virtual void addArtikel(string, string, string, int) = 0;
  virtual void removeArtikel(int, int) = 0;
  virtual vector<Article> listArtikels(int) = 0; 
  virtual vector<Newsgroup> listNewsgroups() = 0;
  virtual Article getArtikel(int, int) = 0;
};

#endif
