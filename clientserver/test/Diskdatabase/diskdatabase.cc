#include "livedatabase.h"
#include <algorithm>
#include <iostream>

using std::string;

void LiveDataBase::addNewsgroup(string name) {
    //Add a check if name already exists
    StoreNewsG.push_back(Newsgroup(name, idNextNewsG));
    idNextNewsG++;
}

void LiveDataBase::removeNewsgroup(long long int id) {
    std::remove_if(StoreNewsG.begin(), StoreNewsG.end(), [&id](Newsgroup ng) {
        return ng.getId() == id;
    });
}

void LiveDataBase::addArtikel(Article a, long long int id) {
    auto it = std::find_if(StoreNewsG.begin(), StoreNewsG.end(), [&id](Newsgroup ng) {
        return ng.getId() == id;
    });
    if (it == StoreNewsG.end()) {
        std::cout << "Now newsgroup with that ID";
    } else {
        (*it).addArticle(a);
    }
}

void LiveDataBase::removeArtikel(long long int NewsGID, long long int articID) {
    auto it = std::find_if(StoreNewsG.begin(), StoreNewsG.end(), [&NewsGID](Newsgroup ng) {
        return ng.getId() == NewsGID;
    });
    if (it == StoreNewsG.end()) {
        std::cout << "Now newsgroup with that ID";
    } else {
        (*it).removeArticle(articID);
    }
}

vector<Article> LiveDataBase::listArtikels(long long int id) {
    auto it = std::find_if(StoreNewsG.begin(), StoreNewsG.end(), [&id](Newsgroup ng) {
        return ng.getId() == id;
    });
    return (*it).getAllArticles();
}

vector<Newsgroup> LiveDataBase::listNewsgroups() {
    return StoreNewsG;
}
