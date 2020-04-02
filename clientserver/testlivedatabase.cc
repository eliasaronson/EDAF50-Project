#include <iostream>
#include <vector>
#include "livedatabase.h"
#include "article.h"
#include "newsgroup.h"

using std::vector;
using std::cout;
using std::endl;

int main() {
    LiveDataBase ldbase;
    Article a1("How to C++", "Aldux Huxley", "Always write a good h-file.", 1);
    Article a2("How to C", "Aldux Huxley", "What is a struct?", 2);
    ldbase.addNewsgroup("Name1");
    ldbase.addNewsgroup("Name2");
    ldbase.addArtikel(a1, 0);
    ldbase.addArtikel(a2, 0);
    vector<Article> listA = ldbase.listArtikels(0);
    vector<Newsgroup> listN = ldbase.listNewsgroups();

    for(auto a : listA){
        cout << a.getTitle() << endl;
    }
}
