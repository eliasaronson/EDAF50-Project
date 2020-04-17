#include "livedatabase.h"
#include "protocol.h"
#include <algorithm>
#include <iostream>

using std::string;

void LiveDataBase::addNewsgroup(string name) {
    auto it = std::find_if(StoreNewsG.begin(), StoreNewsG.end(), [&name](Newsgroup ng) {
        return ng.getName() == name;
    });
    if (it != StoreNewsG.end()) {
        int err = static_cast<int>(Protocol::ERR_NG_ALREADY_EXISTS);
        throw std::to_string(err);
    } else {
        StoreNewsG.push_back(Newsgroup(name, idNextNewsG));
        idNextNewsG++;
    }
}

void LiveDataBase::removeNewsgroup(int id) {
    std::remove_if(StoreNewsG.begin(), StoreNewsG.end(), [&id](Newsgroup ng) {
        return ng.getId() == id;
    });
}

void LiveDataBase::addArtikel(string title, string auth, string text, int id) {
    auto it = std::find_if(StoreNewsG.begin(), StoreNewsG.end(), [&id](Newsgroup ng) {
        return ng.getId() == id;
    });
    if (it == StoreNewsG.end()) {
        int err = static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST);
        throw std::to_string(err);
    } else {
        (*it).addArticle(title, auth, text);
    }
}

void LiveDataBase::removeArtikel(int NewsGID, int articID) {
    auto it = std::find_if(StoreNewsG.begin(), StoreNewsG.end(), [&NewsGID](Newsgroup ng) {
        return ng.getId() == NewsGID;
    });
    if (it == StoreNewsG.end()) {
        int err = static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST);
        throw std::to_string(err);
    } else {
        (*it).removeArticle(articID);
    }
}

vector<Article> LiveDataBase::listArtikels(int id) {
    auto it = std::find_if(StoreNewsG.begin(), StoreNewsG.end(), [&id](Newsgroup ng) {
        return ng.getId() == id;
    });
    if (it == StoreNewsG.end()) {
        int err = static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST);
        throw std::to_string(err);
    } else {
        return (*it).getAllArticles();
    }
}

vector<Newsgroup> LiveDataBase::listNewsgroups() {
    return StoreNewsG;
}

Article LiveDataBase::getArtikel(int NewsGID, int articID) {
    auto it = std::find_if(StoreNewsG.begin(), StoreNewsG.end(), [&NewsGID](Newsgroup ng) {
        return ng.getId() == NewsGID;
    });
    if (it == StoreNewsG.end()) {
        int err = static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST);
        throw std::to_string(err);
    } else {
        return (*it).getArticle(articID);
    }
}
