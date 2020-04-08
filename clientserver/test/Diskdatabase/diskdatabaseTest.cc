#include "diskdatabase.h"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

int main(){
	cout << "current index: " << currentIndex() << endl;
	DiskDataBase d;
	string name("test123");
	string test = "abc123";
	string a = "a";
	string b = "b";
	string c = "c";
	//string e = "e";

	cout << name << endl;
	cout << test << endl;
//	d.addNewsgroup(test);
//	d.addNewsgroup(name);
	d.addNewsgroup(a);
	d.addNewsgroup(b);
	d.addNewsgroup(c);
	d.addNewsgroup(test);
	cout << "In main after d.add..." << endl;
	return 0;
}
