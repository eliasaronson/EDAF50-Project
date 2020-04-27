#include <iostream>
#include <vector>
#include "livedatabase.h"
#include "article.h"
#include "newsgroup.h"

using std::vector;
using std::cout;
using std::endl;


void setup(LiveDataBase& ldbase){
    ldbase.addNewsgroup("Name1");
    ldbase.addNewsgroup("Name2");
    ldbase.addArtikel("How to C++", "Aldux Huxley", "Always write a good h-file.", 0);
    ldbase.addArtikel("How to C", "Terence McKenna", "What is a struct?", 0);
    ldbase.addArtikel("How to C-objective", "Alexander Shulgin", "What is this laguage even?", 0);
    ldbase.addArtikel("How to C#", "Rick Strassman", "How many C laguages are there?", 0);
}

void testAddedArticles(LiveDataBase& ldbase){
    string titles[4] = {"How to C++", "How to C", "How to C-objective", "How to C#"};
    vector<Article> listA = ldbase.listArtikels(0);
    int i = 0;
    bool err = false;
    for (auto a : listA) {
      if(a.getTitle() != titles[i]){ 
          err = true;
          cout << "Name did not match. " << a.getTitle() << "Expected: " << titles[i] << endl;
      }
      if (err) {
        cout << "Titles correct" << endl;
      }
      i++;
    }
}

void testAddingNGSameName(LiveDataBase& ldbase){
  try {
      ldbase.addNewsgroup("Name1");
   } catch (string msg) {
     cout << msg << endl;
   }
#if 1
    vector<Newsgroup> listN = ldbase.listNewsgroups();
    for(auto ns : listN){
      cout << ns.getName() << " " << ns.getId() << endl;
    }
#endif
}

void testRemovingNoId(LiveDataBase& ldbase){
  try {
      ldbase.removeArtikel(1, 15);
   } catch (const char* msg) {
     cout << msg << endl;
   }
}

int main() {
    LiveDataBase ldbase;
    setup(ldbase);
    testAddedArticles(ldbase);
    testAddingNGSameName(ldbase);
    testRemovingNoId(ldbase);
    vector<Article> listA = ldbase.listArtikels(0);
    vector<Newsgroup> listN = ldbase.listNewsgroups();

    for(auto a : listA){
        cout << "Newsgroup: " << listN[0].getName() << endl;
        cout << "Title: " << a.getTitle() << endl;
        cout << "Author: " << a.getAuthor() << endl;
        cout << "Text: " << a.getText() << endl;
    }
}
