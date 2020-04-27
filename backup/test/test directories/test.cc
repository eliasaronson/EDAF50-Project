#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

int main() {
   int status;
   status = system("mkdir -p ~/EDAF50-Project/New_folder_test"); // Creating a directory
   if (status == -1)
      cerr << "Error : " << strerror(errno) << endl;
   else
      cout << "Directories are created" << endl;
}
