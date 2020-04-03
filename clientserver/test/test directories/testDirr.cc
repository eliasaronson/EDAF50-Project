//Test program for making/deleting directories.
#include <fstream>
#include <string>
#include <bits/stdc++.h> 	//to make dirr
#include <iostream> 
#include <sys/stat.h> 		//to make dirr
#include <sys/types.h>		//to make dirr and open
#include <dirent.h> 		//to open dirr
#include <libgen.h>			//to open dirr
#include <sstream>      	//std::stringstream

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;

boolean writeNewsgroup(string &newsgroupname){
	//add which directory it should be saved in, currently saved i current dirr. with input name
	//Use form /usr/dirr1/newsgroupname
	string path(newsgroupname);		
	if (mkdir(newsgroupname, 0) == -1) {
        cerr << "Error :  " << strerror(errno) << endl; 
		return false;
	}
    else{
        cout << "Directory created i.e. Newsgroup: " << newsgroupname;
		return true;
	}
}

void saveArticle(string &newsgroupname, istream &article){
	//same path as write newsgroup
	string path(newsgroupname);
	DIR *dir;
    struct dirent *dp;
	if ((dir = opendir (path)) == NULL) {
		cerr << "Error : Can't open dirr " << path << endl;
        exit(1);
    } 
	else if ((dp = readdir (dir)) != NULL){
		//use stringstream to read file. Assume first row is unique id, second row name, rest is article
		stringstream ss;
		int id;
		string artName;
		ss >> id >> artName;
		if (ss.fail()){
			cerr << "Error : wrong format on input stream" << endl;
			closedir(dir);		//to not leave directory open
			exit(1);
		}
		
		ofstream myfile(id);	//if not creating file add arguments:  (fstream::in | fstream::out | fstream::trunc)
		if (myfile.is_open()){
			myfile << id << "\n";
			string word;
			while (ss.get(word)){
				myfile << word << " ";
			}
			if (ss.eof()){
				cout << "File saved successfully" << endl;
			} else {
				cerr << "Error : Can't read file" << endl;
				closedir(dir);		//to not leave directory open
				exit(1);
			}
			file.close();
		} else {
			cerr << "Error : Can't create file id: " << id << endl;
			closedir(dir);			//to not leave directory open
			exit(1);
		}
		closedir(dir);
	}

}

int main(){
	//On disk database, store newsgroups in vector<string> and articles as objects.
	string newsgroupname(example);
	writeNewsgroup(newsgroupname);
	//save article with filename id (unique) use object id in database, articles use shared id counter cross objects?
	istream stream();
	saveArticle(newsgroupname, stream);
	exit(0);
}