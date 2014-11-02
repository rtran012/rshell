#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

int aflag = 0;
int lflag = 0;
int Rflag = 0;

void output_ls(char * dirName){
	
	DIR *currPath;
	struct dirent * currFile;
	
	currPath = opendir(dirName);
	if(currPath == NULL){
			perror("opendir: ");
		
	}
	else{
		while((currFile = readdir(currPath)) != NULL){
			if(currFile->d_name[0] != '.'){
				cout << "currFile " << currFile->d_name << endl;
			}
			else if (aflag == 1 && currFile->d_name[0] == '.'){
				cout << "currFile " << currFile->d_name << endl;
			}

		}
		closedir(currPath);
	}

	
}

int main(int argc, char**argv)
{

	int index;
	int c;
	
	opterr = 0;
	
	while((c = getopt(argc, argv, "alR")) != -1){
		switch(c){
			case 'a':
				aflag = 1;
				break;
			case 'l':
				lflag = 1;
				break;
			case 'R':
				Rflag = 1;
				break;
			default:
				abort();
		}
	}
	
	cout << "a =" << aflag << endl;
	cout << "l =" << lflag << endl;
	cout << "R =" << Rflag << endl;
	for(index = optind ; index < argc; index++)
		cout<< index << " Non-option argument " << argv[index] << endl ;	
	//assume at least one argument
	if(optind == argc) {
		char dir_p[2];
		strcpy(dir_p, ".");
		output_ls(dir_p);
	}
	else {
		for(index = optind ; index < argc; index++)
			output_ls(argv[index]);
	}
	return 0;
}








