#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

int main() {
   int status;
   status = system("mkdir -p /Documents/C++/EDAF50-Project/New-folder-test"); // Creating a directory
   if (status == -1)
      cerr << "Error : " << strerror(errno) << endl;
   else
      cout << "Directories are created" << endl;
}
