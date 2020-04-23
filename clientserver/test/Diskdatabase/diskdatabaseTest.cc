#include "diskdatabase.h"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

int main(){
	int tempid = 1;
	int returntempid = nextArtId(tempid);
	cout << "next art id in main: " << returntempid << endl;
	cout << "current index: " << currentIndex() << endl;
	DiskDataBase d;
	string name("test123");
	string test = "abc123";
	string a = "a";
	string b = "b";
	string c = "c";
	string e = "e";

	cout << name << endl;
	cout << test << endl;
//	d.addNewsgroup(test);
//	d.addNewsgroup(name);
	d.addNewsgroup(a);
	d.addNewsgroup(b);
	d.addNewsgroup(c);
	d.addNewsgroup(test);
	cout << "In main after d.add..." << endl;
	int id = 2;
	//d.removeNewsgroup(id);
	d.addNewsgroup(e);
	int grp3 = 3;
	string title("title");
	string auth("auth");
	string text("text");
	d.addArticle(title,auth,text,grp3);
	d.addArticle(title,auth,text,grp3);
	d.addArticle(title,auth,text,grp3);
	d.addArticle(title,auth,text,grp3);
	d.addArticle(title,auth,text,grp3);
	d.addArticle(title,auth,text,grp3);
	d.removeArticle(grp3,grp3);
	d.removeArticle(grp3,id);
	return 0;
}
