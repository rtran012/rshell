#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

int aflag = 0;
int lflag = 0;
int Rflag = 0;

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
	return 0;
}
