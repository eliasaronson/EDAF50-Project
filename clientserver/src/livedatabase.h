#ifndef LIVEDATABASE_H
#define LIVEDATABASE_H

#include <vector>
#include <string>
#include "newsgroup.h"
#include "article.h"
#include "database.h"

using std::vector;
using std::string;

class LiveDataBase : public DataBase{
public:
  LiveDataBase() = default;
  ~LiveDataBase() = default;
  void addNewsgroup(string) override;
  void removeNewsgroup(int) override;
  void addArtikel(string, string, string, int) override;
  void removeArtikel(int, int) override;
  vector<Article> listArtikels(int) override;
  vector<Newsgroup> listNewsgroups() override;
  Article getArtikel(int, int) override;
private:
  vector<Newsgroup> StoreNewsG; 
  int idNextNewsG{};
};

#endif
