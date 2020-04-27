
#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

using std::string;

class Article {
public:
  Article(string, string, string, long long int);
  Article(const Article& art);
  string getTitle();
  string getAuthor();
  string getText();
  long long int getId();
private:
  string title;
  string author; 
  string text; 
  long long int id{};
};

#endif
