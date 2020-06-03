#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <vector>
#include "article.h"

using std::string;
using std::vector;

class Newsgroup {
public:
  Newsgroup(string, long long int);
  void addArticle(string, string, string);
  void removeArticle(long long int);
  long long int getId();
  string getName();
  vector<Article> getAllArticles();
  Article getArticle(long long int);
private:
  string name;
  long long int id;
  long long int articNextId{};
  vector<Article> store;
  long long int idPreviousArticle{};
};

#endif
