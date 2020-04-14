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

//returns the highest file number(article id)+1 in the directory with id grpId
int nextArtId(size_t& grpId){
	string grpName = getName(grpId); 
	//find home/username path
	struct passwd *pw = getpwuid(getuid());
	const char *homedir = pw->pw_dir;
	string homeDir(homedir);
	//opendir needs full path, ~/Database/... does not work
	string path(homeDir + "/Database/" + grpName);
	struct dirent *entry;
	DIR *dir = opendir(path.c_str());
	//Mostly for finding errors
	if (errno == ENOENT){
		cout << "opendir failed: " << std::strerror(errno) << endl;
	}

	if (dir == NULL){
		return -1;
	}

	//find last used ID
	//reads hidden files as well
	int id = 0;
	while ((entry = readdir(dir)) != NULL) {
		id = std::atoi(entry->d_name);
	}
	closedir(dir);
	
	cout << "id in nextArtId: " << id << endl;
	id++;
	return id;
}

void DiskDataBase::addArticle(string& title, string& auth, string& text, size_t& grpId){
	string grpName = getName(grpId);
	if (grpName.compare("ERRORIdNotConnectToName")==0){
		throw "No newsgroup with this id";
	} else {
		int artId = nextArtId(grpId);
		if (artId == -1){
			throw "can't open directory";
		}
		struct passwd *pw = getpwuid(getuid());
		const char *homedir = pw->pw_dir;
		string homeDir(homedir);
		string path(homeDir + "/Database/" + grpName + "/" + std::to_string(artId));
		std::ofstream outFile(path);
		if (!outFile.is_open()){
			cout << "Error : outFile isn't open" << endl;
		}
		outFile << title << '\n' << auth << '\n' << text;
		outFile.close();
		cout << "outFile " << std::to_string(artId) << " closed" << endl;
	}
}

//newsgroup must exist to call this
bool containsArticle(size_t& grpId, size_t& artId){
	string grpName = getName(grpId); 
	//find home/username path
	struct passwd *pw = getpwuid(getuid());
	const char *homedir = pw->pw_dir;
	string homeDir(homedir);
	//opendir needs full path, ~/Database/... does not work
	string path(homeDir + "/Database/" + grpName);
	struct dirent *entry;
	DIR *dir = opendir(path.c_str());
	//Mostly for finding errors
	if (errno == ENOENT){
		cout << "opendir failed: " << std::strerror(errno) << endl;
	}

	if (dir == NULL){
		return 0;
	}

	//Check if ID exists
	//reads hidden files as well
	int articleId = static_cast<int>(artId);
	int id = 0;

	//add to handle hidden file . and ..?
	while ((entry = readdir(dir)) != NULL) {
		id = std::atoi(entry->d_name);
		if (id == articleId){
			closedir(dir);
		       return true;
		}	       
	}
	closedir(dir);
	return false;
}

void DiskDataBase::removeArticle(size_t& groupId, size_t& artId) {
	string grpName = getName(groupId);
	if (grpName.compare("ERRORIdNotConnectToName")==0){
		cout << "Error : can't remove article: newsgroup doesn't exist" << endl;
		throw "No newsgroup with this id";
	} else if (!containsArticle(groupId, artId)){
		cout << "Error : can't remove article: article doesn't exists" << endl;
		throw "No Article with this id";
	}
	else {
		string command("rm ~/Database/" + grpName + "/" + std::to_string(artId));
		int status = system(command.c_str());
		if (status == -1){
        		cerr << "Error :  " << strerror(errno) << endl; 
			throw "Error removing article";
		}
	}
}
/*
vector<Article> LiveDataBase::listArtikels(long long int id) {

    }

vector<Newsgroup> LiveDataBase::listNewsgroups() {

}*/
