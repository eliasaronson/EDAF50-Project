#include "diskdatabase.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

//Should only be called if id.txt is closed(?)
int currentIndex(){
	//should be ~/Database/id.txt
	std::ifstream fin("id.txt");
	int id;
	if (fin.is_open()){
		fin.seekg(-1,std::ios_base::end);			//go to one spot before the EOF
		bool keepLooping = true;
		while (keepLooping){
			char ch;
			fin.get(ch);					//get current byte's data

			if((int)fin.tellg() <= 1){			//if data was at or before 0th byte first line is last, stop
				fin.seekg(0);
				keepLooping = false;
			}
			else if(ch == '\n'){
				keepLooping = false;			//if data was newline this is the last row	
				fin.seekg(-2,std::ios_base::cur);	//last line in file marked by '\n', however an extra is added due to this implementation.
			}
			else {
				fin.seekg(-2,std::ios_base::cur);	//if data was neither newline nor at 0 move one step up
			}
		}

		string lastLine;
		getline(fin,lastLine);
		id = std::stoi(lastLine);					//For some reason lastline is only index on last row
		//std::stringstream ss(lastLine);
		//string temp;
		//ss >> temp >> id;
//		cout << "The last line in id.txt is: " << lastLine << endl;
		fin.close();
	}
	return id;
}

string getId_txt(){
	//should be ~/Database/id.txt
	std::ifstream file("id.txt");
	string txt;
	string line;
	if (file.is_open()){
		while (file.good() && !file.eof()){
			getline(file, line);
			txt += line + "\n";
		}
	}
	file.close();
	return txt;
}

bool containsWord(const string& name){
	string txt = getId_txt();
	cout << "txt: " << txt << endl;
	std::stringstream ss(txt);
	int tempId;
	string word;
	cout << "before ss.good" << endl;
	//Seems like this loops runs to many times (one to many?) Not anymore?
	while (!ss.eof() && ss.good()){
		cout << "ss.good() = true" << endl;
		ss>>word;
		ss>>tempId;
		cout << "word : " << word << " name : " << name << endl;
		if (/*ss.good() &&*/ word.compare(name) == 0){
			cout << "containsWord returns true" << endl;
			return true;
		} 
	}
	return false;
}

void DiskDataBase::addNewsgroup(const string& name) {
	if (containsWord(name)){
		cerr << "Error : Newsgroup already exists" << endl;
		//throw "Newsgroup already exists";
	} else {
		//Create the actual directory
		string path("mkdir -p ~/Database/" + name);		
		int status = system(path.c_str());
		if (status == -1) {
        		cerr << "Error :  " << strerror(errno) << endl; 
			throw "Error creating folder";
		}
		else{
			//Add name and id to id.txt
			int nextId = currentIndex()+1;
			//should be ~/Database/id.txt
			std::ofstream outFile("id.txt",std::ios_base::app);
			outFile << name + " " << nextId << "\n";
			outFile.close();
			//For debugging
			cout << "Directory created i.e. Newsgroup: " << name << endl;
			cout << "id for created newsgroup: " << nextId << endl;
		}
	}
}

string getName(size_t& groupId){
	std::stringstream ss(getId_txt());
	size_t tempId;
	string word;
	while (!ss.eof() && ss.good()){
		ss>>word;
		ss>>tempId;
		if (groupId == tempId){
			return word;
		} 
	}
	return "ERRORIdNotConnectToName";

}

void DiskDataBase::removeNewsgroup(size_t& groupId) {
	string name = getName(groupId);
	if (name.compare("ERRORIdNotConnectToName")==0){
		throw "No newsgroup with this id";
	} else {
		string path("rm -r ~/Database/" + name);
		int status = system(path.c_str());
		if (status == -1) {
        		cerr << "Error :  " << strerror(errno) << endl; 
			throw "Error removing folder";
		}
		else{
			//should be ~/Database/id.txt
			std::ifstream inFile("id.txt");
			//should be ~/Database/temp.txt
			std::ofstream outFile("temp.txt", std::ofstream::out);
			std::stringstream ss(getId_txt());
			size_t tempId;
			string word;
			while (!ss.eof() && ss.good()){
				ss>>word;
				if (ss.eof()){
					break;
				}
				ss>>tempId;
				if (groupId == tempId && word.compare(name)==0){
					cout << "matching word: " << word << " Name to remove: " << name << endl;
					cout << "matching id: " << tempId << " Id to remove: " << groupId << endl;
				} else {
				       outFile << word + " " << tempId << "\n";
				}
			}
			inFile.close();
			outFile.close();

			//should be ~/Database/id.txt
			remove("id.txt");
			//should be ~/Database/temp.txt
			rename("temp.txt", "id.txt");
		}
	}
}

void DiskDataBase::addArticle(string& article, size_t& artId) {

}

void DiskDataBase::removeArticle(size_t& groupId, size_t& artId) {
    
}
/*
vector<Article> LiveDataBase::listArtikels(long long int id) {

    }

vector<Newsgroup> LiveDataBase::listNewsgroups() {

}*/
