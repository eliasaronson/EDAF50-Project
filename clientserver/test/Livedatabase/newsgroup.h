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
  void addArticle(Article);
  void removeArticle(long long int);
  long long int getId();
  string getName();
  vector<Article> getAllArticles();
private:
  string name;
  long long int id;
  vector<Article> store;
  long long int idPreviousArticle{};
};

#endif
