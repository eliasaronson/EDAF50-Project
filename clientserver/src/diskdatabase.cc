#include "diskdatabase.h"
#include "protocol.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::regex;
using std::stringstream;

DiskDataBase::DiskDataBase(){
  char temp[200];
  getcwd(temp,200);
  char* pch = temp;
  pch[strlen(pch)-4]=0;
  string path(temp);
	struct dirent *entry;
	DIR *dir = opendir(path.c_str());

	if (dir == NULL){
		cout << "opendir failed: " << std::strerror(errno) << endl;
		throw std::runtime_error("can't open directory");
	}

  bool DbFound = false;

  while ((entry = readdir(dir)) != NULL) {
    //this needs to be able to handle . and .. file
    string file(entry->d_name);
    if (file == "Database") {
      DbFound = true;
    }
  }
  if (!DbFound) {
    int status = mkdir(path.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (status == -1) {
      //unexpected error creating folder
      cerr << "Error :  " << strerror(errno) << endl; 
		  throw std::runtime_error(strerror(errno));
    }
  }
}

//Assumes the code is executed from .../bin and Database location should be ../Database
string getDbDir(){
  //needs to be big enough for full directory path
  char path[200];
  getcwd(path,200);
  char* pch = path;
  pch[strlen(pch)-3]=0;
  string ret(path);
  ret += "Database/";
  return ret;
}

//Should only be called if id.txt is closed(?)
int currentIndex(){
	//should be .../Database/id.txt
  string path(getDbDir() + "id.txt");
	std::ifstream fin(path.c_str());
	int id;
	if (fin.is_open()){
		fin.seekg(-1,std::ios_base::end);			//go to one spot before the EOF
		bool keepLooping = true;
		while (keepLooping){
			char ch;
			fin.get(ch);					//get current byte's data

			if(static_cast<int>(fin.tellg()) <= 1){			//if data was at or before 0th byte first line is last, stop
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
    cout << "current Index from currentIndex(): " << id << endl;
		fin.close();
	}
	return id;
}

//obsolete function now when containsword and getname is changed?
string getId_txt(){
	//should be ~/Database/id.txt
  string path(getDbDir() + "id.txt");
	std::ifstream file(path.c_str());
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
  int tempId;
	string word;

  string path(getDbDir() + "id.txt");
	std::ifstream file(path.c_str());
	string txt;
	string line;
	if (file.is_open()){
		while (file.good() && !file.eof()){
			getline(file, line);
      string revLine = line;
      reverse(revLine.begin(),revLine.end());
      stringstream ss(revLine);
      ss >> tempId;
      string regx = R"(#([\s\S]*?)#)";
      std::smatch matches;
      if (regex_search(line, matches, regex(regx)))
      {
        word = matches[1];
      }
      cout << "ContainsWord is searching for : " << name << " but found: " << word << endl;
		  if (name == word){
        file.close();
        cout << "ContainsWord returns true" << endl;
        return true;
		  } 
		}
	}
  file.close();
  cout << "ContainsWord returns false" << endl;
  return false;
}

void DiskDataBase::addNewsgroup(string name) {
	if (containsWord(name)){
		cerr << "Error : Newsgroup already exists" << endl;
    int err = static_cast<int>(Protocol::ERR_NG_ALREADY_EXISTS);
		throw err;
	} else {
    string NGname = "'" + name + "'";
    cout << "Newsgrop name with apostrhopes: " << NGname << endl;
		//Create the actual directory
    
		//string path("mkdir -p ../Database/" + NGname);		
		//int status = system(path.c_str());
    string path(getDbDir() + "/" + name);
    int status = mkdir(path.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (status == -1) {
      //unexpected error creating folder
     	cerr << "Error :  " << strerror(errno) << endl; 
			throw std::runtime_error(strerror(errno));
		}
		else{
			//Add name and id to id.txt
			int nextId = currentIndex()+1;
			//should be ~/Database/id.txt
      string path2(getDbDir() + "id.txt");
			std::ofstream outFile(path2.c_str(),std::ios_base::app);
      string IdName = "#" + name + "#";
			outFile << IdName + " " << nextId << "\n";
			outFile.close();
			//For debugging
			cout << "Directory created i.e. Newsgroup: " << name << endl;
			cout << "id for created newsgroup: " << nextId << endl;
		}
	}
}

string getName(int& groupId){
	int tempId;
	string word;

  string path(getDbDir() + "id.txt");
	std::ifstream file(path.c_str());
	string txt;
	string line;
	if (file.is_open()){
		while (file.good() && !file.eof()){
			getline(file, line);
      string revLine = line;
      reverse(revLine.begin(),revLine.end());
      stringstream ss(revLine);
      ss >> tempId;

      string regx = R"(#([\s\S]*?)#)";
      std::smatch matches;
      if (regex_search(line, matches, regex(regx)))
      {
        word = matches[1];
      }
		  if (groupId == tempId){
        file.close();
        cout << "Group id: " << groupId << " matches with ng name: " << word << endl;
			  return word;
		  } 
		}
	}
  file.close();
	return "ERRORIdNotConnectToName";
}

void DiskDataBase::removeNewsgroup(int groupId) {
	string name = getName(groupId);
	if (name.compare("ERRORIdNotConnectToName")==0){
    int err = static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST);
		throw err;
	} else {
    string NGname = "'" + name + "'";
		string path("rm -r ../Database/" + NGname);
		int status = system(path.c_str());
		if (status == -1) {
     	cerr << "Error :  " << strerror(errno) << endl; 
			throw std::runtime_error("Error removing folder");
		}
		else{
			//should be ../Database/id.txt
      string path2(getDbDir() + "id.txt");
			std::ifstream inFile(path2.c_str());
			//should be ../Database/temp.txt
      string path3(getDbDir() + "temp.txt");
			std::ofstream outFile(path3.c_str(), std::ofstream::out);
      
      int tempId;
	    string word;

	    string txt;
	    string line;
	    if (inFile.is_open()){
		    while (inFile.good() && !inFile.eof()){
			    getline(inFile, line);
          string revLine = line;
          reverse(revLine.begin(),revLine.end());
          stringstream ss(revLine);
          ss >> tempId;

          string regx = R"(#([\s\S]*?)#)";
          std::smatch matches;
          if (regex_search(line, matches, regex(regx))){
            word = matches[1];
            if (groupId == tempId && word.compare(name)==0) {
					    cout << "matching word: " << word << " Name to remove: " << name << endl;
					    cout << "matching id: " << tempId << " Id to remove: " << groupId << endl;
            }
            else {
              string IdWord = "#" + word + "#";
				      outFile << IdWord + " " << tempId << "\n";
            }
          }
		    }
	    }
  		inFile.close();
			outFile.close();

			//should be ../Database/id.txt
			remove(path2.c_str());
			//should be ../Database/temp.txt
			rename(path3.c_str(), path2.c_str());
		}
	}
}

//returns the highest file number(article id)+1 in the directory with id grpId
int nextArtId(int& grpId){
	string grpName = getName(grpId); 
	//opendir needs full path, ~/Database/... does not work
	string path(getDbDir() + grpName);
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
    int temp = std::atoi(entry->d_name);
      if (temp > id) {
        id = temp;
      }
	}
	closedir(dir);
	
	cout << "id in nextArtId: " << id << endl;
	id++;
	return id;
}

void DiskDataBase::addArtikel(string title, string auth, string text, int grpId) {
	string grpName = getName(grpId);
	if (grpName.compare("ERRORIdNotConnectToName")==0){
    int err = static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST);
		throw err;
	} else {
		int artId = nextArtId(grpId);
		if (artId == -1){
			throw std::runtime_error("Can't open directory");
		}
		string path(getDbDir() + grpName + "/" + std::to_string(artId));
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
Article* containsArticle(int& grpId, int& artId) {
	string grpName = getName(grpId); 
  if (grpName.compare("ERRORIdNotConnectToName")==0){
    int err = static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST);
		throw err;
	}
	//opendir needs full path, ~/Database/... does not work
	string path(getDbDir() + grpName);
	struct dirent *entry;
	DIR *dir = opendir(path.c_str());

	if (dir == NULL){
		cout << "opendir failed: " << std::strerror(errno) << endl;
		return NULL;
	}

	//Check if ID exists
	//reads hidden files as well
	int articleId = artId;
	int id = 0;

	//add to handle hidden file . and ..?
	while ((entry = readdir(dir)) != NULL) {
		id = std::atoi(entry->d_name);
		if (id == articleId){
      std::ifstream art(path + "/" + entry->d_name);
      string title, auth, text;
      getline(art, title);
      getline(art, auth);
      string line;
      while (art.good() && !art.eof()){
        std::getline(art, line,'\n');
        if (art.eof()) {
			    text += line;
        } else {
			    text += line + "\n";
        }
		  }
			closedir(dir);
      Article* ret = new Article(title,auth,text,id);
		  return ret;
		}	       
	}
	closedir(dir);
	return NULL;
}

void DiskDataBase::removeArtikel(int groupId, int artId) {
	string grpName = getName(groupId);
	if (grpName.compare("ERRORIdNotConnectToName")==0){
		cout << "Error : can't remove article: newsgroup doesn't exist" << endl;
    int err = static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST);
		throw err;
	} else if (containsArticle(groupId, artId) == NULL){
		cout << "Error : can't remove article: article doesn't exists" << endl;
    int err = static_cast<int>(Protocol::ERR_ART_DOES_NOT_EXIST);
		throw err;
	}
	else {
    string NGname = getDbDir() + grpName; 
    cout << "NGname: " << NGname << endl;
		string command("rm '" + NGname + "/" + std::to_string(artId) + "'");
    cout << "about to do comand: " << command << endl;
		int status = system(command.c_str());
		if (status == -1){
      //unexpected error when removing directory
     	cerr << "Error :  " << strerror(errno) << endl; 
			throw std::runtime_error("Error removing article");
		}
	}
}

vector<Article> DiskDataBase::listArtikels(const int id) {
  int tempId = static_cast<int>(id);
  string grpName = getName(tempId); 
  if (grpName.compare("ERRORIdNotConnectToName")==0){
		cout << "Error : can't list articles: newsgroup doesn't exist" << endl;
    int err = static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST);
		throw err;
	}
	//opendir needs full path, ~/Database/... does not work
  string NGname = getDbDir() + grpName; 
	string path(NGname);
	struct dirent *entry;
	DIR *dir = opendir(path.c_str());

	if (dir == NULL){
		cout << "opendir failed: " << std::strerror(errno) << endl;
		throw std::runtime_error("can't open directory");
	}

  vector<Article> ret;
  char dotfile[3] = {'.', '\0'};
  char dotdotfile[3] = {'.', '.', '\0'};

  while ((entry = readdir(dir)) != NULL) {
    //this needs to be able to handle . and .. file
    char* file = entry->d_name;
    if (strcmp(file,dotfile) != 0 && strcmp(file,dotdotfile) != 0) {
      int artId = std::atoi(entry->d_name);
      string title; string auth; string text;
      //requires c-string
      string path2(path + '/' + std::to_string(artId));
      std::ifstream inFile(path2.c_str(), std::ifstream::in);
      std::getline(inFile,title,'\n');
      std::getline(inFile,auth,'\n');
      
      string line;
      while (inFile.good() && !inFile.eof()){
        std::getline(inFile, line,'\n');
			  text += line + "\n";
		  }
      if (title.length() != 0) {
        //ret.emplace(ret.begin()+artId,title,auth,text,artId);
        ret.emplace_back(title,auth,text,artId);
      }
      inFile.close();
    }
  }
  closedir(dir);
  sort(ret.begin(),ret.end(),[] (Article& a, Article& b) -> bool {return (a.getId()<b.getId());});
  return ret;
}

vector<Newsgroup> DiskDataBase::listNewsgroups() {
  vector<Newsgroup> ret;
  int tempId;
	string word;

  string path(getDbDir() + "id.txt");
	std::ifstream file(path.c_str());
	string txt;
	string line;
	if (file.is_open()){
		while (file.good() && !file.eof()){
			getline(file, line);
      string revLine = line;
      reverse(revLine.begin(),revLine.end());
      stringstream ss(revLine);
      ss >> tempId;

      string regx = R"(#([\s\S]*?)#)";
      std::smatch matches;
      if (regex_search(line, matches, regex(regx)))
      {
        word = matches[1];
        if (word.length() >= 1) {
          cout << "listNG is adding: " << word << " with id: " << tempId << "to ret" << endl;
          ret.emplace_back(word,static_cast<long long int>(tempId));
        }
      }
		}
	}
  file.close();
  cout << "listNG returns vector ret" << endl;
  return ret;
}

Article DiskDataBase::getArtikel(int grpId, int artId) {
  string grpName = getName(grpId); 
  if (grpName.compare("ERRORIdNotConnectToName")==0){
		cout << "Error : can't list articles: newsgroup doesn't exist" << endl;
    int err = static_cast<int>(Protocol::ERR_NG_DOES_NOT_EXIST);
		throw err;
	}
  Article* retp = containsArticle(grpId,artId);
  if (retp != NULL) {
    Article cpy(*retp);
    delete retp;
    return cpy;
  }
  else {
    int err = static_cast<int>(Protocol::ERR_ART_DOES_NOT_EXIST);
    throw err;
  }
}
