#include "newsgroup.h"
#include <algorithm>
#include <iostream>



Newsgroup::Newsgroup(string name, long long int id) : name{name}, id{id} {}

void Newsgroup::addArticle(Article a){
  store.push_back(a); 
}

void Newsgroup::removeArticle(long long int id){

   auto it = std::find_if(store.begin(), store.end(), [&id](Article a) { return a.getId() == id;}); 
   if (it == store.end()) {
     std::cout << "Now article with that ID";
   } else{
     store.erase(it);
   }
   
}

long long int Newsgroup::getId(){
  return id;
}

vector<Article> Newsgroup::getAllArticles(){
  return store;
}
