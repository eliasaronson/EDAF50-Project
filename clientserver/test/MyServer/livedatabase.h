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
  void removeNewsgroup(long long int) override;
  void addArtikel(string, string, string, long long int) override;
  void removeArtikel(long long int, long long int) override;
  vector<Article> listArtikels(long long int) override;
  vector<Newsgroup> listNewsgroups() override;
  Article getArtikel(long long int, long long int) override;
private:
  vector<Newsgroup> StoreNewsG; 
  long long int idNextNewsG{};
};

#endif
