#include <iostream>
#include <ctype.h>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <string>
#include <sys/types.h>
#include <cctype>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <sys/stat.h>
#include <libgen.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>

using namespace std;

int aflag = 0;
int lflag = 0;
int Rflag = 0;

bool compareNoCase(const string& s1, const string& s2){
	return strcasecmp( s1.c_str(), s2.c_str()) <= 0;
}

void mode_to_letters( int mode, char str[] ){
	strcpy( str, "----------" );           /* default=no perms */

	if ( S_ISDIR(mode) )  str[0] = 'd';    /* directory?       */
	if ( S_ISCHR(mode) )  str[0] = 'c';    /* char devices     */
	if ( S_ISBLK(mode) )  str[0] = 'b';    /* block device     */
	if ( S_ISLNK(mode) )  str[0] = 'l';    /* symbolic link    */
	
	if ( mode & S_IRUSR ) str[1] = 'r';    /* 3 bits for user  */
	if ( mode & S_IWUSR ) str[2] = 'w';
	if ( mode & S_IXUSR ) str[3] = 'x';
	
	if ( mode & S_IRGRP ) str[4] = 'r';    /* 3 bits for group */
	if ( mode & S_IWGRP ) str[5] = 'w';
	if ( mode & S_IXGRP ) str[6] = 'x';

	if ( mode & S_IROTH ) str[7] = 'r';    /* 3 bits for other */
	if ( mode & S_IWOTH ) str[8] = 'w';
	if ( mode & S_IXOTH ) str[9] = 'x';
}

char *uid_to_name( uid_t uid ){
	struct	passwd *pw_ptr;
	static  char numstr[10];

	if ( ( pw_ptr = getpwuid( uid ) ) == NULL ){
		sprintf(numstr,"%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name ;
}


char *gid_to_name( gid_t gid ){
	struct group *grp_ptr;
	static  char numstr[10];

	if ( ( grp_ptr = getgrgid(gid) ) == NULL ){
		sprintf(numstr,"%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}
	
int get_total(vector<string> fileList){
	int total = 0;
	for(unsigned int i =0; i < fileList.size(); i++){
		struct stat info_p;
		if(stat(fileList.at(i).c_str() , & info_p) == -1)
			perror("stat: " );
		total += (int)info_p.st_blocks;
	}
	return (total / 2);
}

void print_ls(vector<string> fileList, int maxLength){

	int maxLine = 78;
	unsigned int ncolum, nlines;
	char format_file[1024];
	char spaceC[2];
	strcpy(spaceC, " ");
	
	
	if(lflag == 0){
		ncolum = ((maxLine-2) / (maxLength+2));
		if(ncolum == 0) ncolum = 1;
		nlines = (fileList.size() + ncolum -1) / ncolum ;
		for(unsigned int i = 0 ; i < nlines ; i++){
			for(unsigned int k = 0 ; k < ncolum; k++){
				if(i+nlines*k < fileList.size()){
					unsigned found = fileList.at(i+nlines*k).find_last_of("/");
					strcpy(format_file, fileList.at(i+nlines*k).substr(found+1).c_str());
					
					struct stat info_p;
					if(stat(fileList.at(i + nlines * k ).c_str(), &info_p) == -1) 
						perror("stat: " );
					if( info_p.st_mode & S_IXUSR || 
					    info_p.st_mode & S_IXGRP ||
					    info_p.st_mode & S_IXOTH  ) printf("\x1b[32m");  
					if( S_ISDIR(info_p.st_mode)) printf("\x1b[34m");
					if( format_file[0] == '.' ) printf("\x1b[47m");

					printf("%s", format_file);
					printf("\x1b[0m");

					if(k <= ncolum -1){
						for(unsigned int j = 0; j < maxLength +2 - strlen(format_file); j++)
							printf("%s", spaceC); 
					}
				}
			}
			printf("\n");
				
		}
	}
	else if(lflag == 1){
		printf("total %d \n", get_total(fileList));	
		for(unsigned int i = 0; i < fileList.size(); i ++){
			struct stat info_p;
			if(lstat(fileList.at(i).c_str() , &info_p) == -1)
				perror("stat: ");
			else{
       		 		char    modestr[11];
				char    fName[1024];
				char 	lName[1024];

				mode_to_letters( info_p.st_mode, modestr );
	
				printf( "%s"    , modestr );
				printf( "%4d "  , (int) info_p.st_nlink);	
				printf( "%-8s " , uid_to_name(info_p.st_uid) );
				printf( "%-8s " , gid_to_name(info_p.st_gid) );
				printf( "%8ld " , (long)info_p.st_size);
				printf( "%.12s ", 4+ctime(&info_p.st_mtime));
				unsigned found = fileList.at(i).find_last_of("/");
				strcpy(fName, fileList.at(i).substr(found+1).c_str());
				if( info_p.st_mode & S_IXUSR || 
				    info_p.st_mode & S_IXGRP ||
				    info_p.st_mode & S_IXOTH  ) printf("\x1b[32m");  
				if( S_ISDIR(info_p.st_mode)) printf("\x1b[34m");
				if( fName[0] == '.' ) printf("\x1b[47m");
				printf( "%s", fName);
				if( S_ISLNK(info_p.st_mode)){
					ssize_t istat;
					strcpy(fName, fileList.at(i).c_str());
					istat = readlink(fName, lName, info_p.st_size+1);
					if(istat == -1) perror("readlink: ");
					lName[istat] = '\0';
					printf( " -> %s", lName);
				}
				printf( "\x1b[0m" );
				printf( "\n");
			} 
		}
	}
		
}
void output_ls(char * dirName, vector<string> * fileList, int * lengthMax, queue <string> * dirList){
	
	int ilen;
	DIR *currPath;
	struct dirent * currFile;
	char buffer[1024];

	currPath = opendir(dirName);
	if(currPath == NULL){
			perror("opendir: ");
	}
	else{
		while((currFile = readdir(currPath)) != NULL){
			if(currFile->d_name[0] != '.' || aflag ==1 ){
				sprintf(buffer, "%s/%s", dirName, currFile->d_name);
				fileList->push_back(buffer);
				ilen= strlen(currFile->d_name);
				if(*lengthMax < ilen) *lengthMax= ilen;
			}
		}
		closedir(currPath);
	}
}

void look_dir(vector<string> fileList, queue <string> * dirList){
	char buffer[1024];
	char fName[1024];
	struct stat info_p;
	
	
	for(unsigned int i = 0; i < fileList.size(); i++){
		unsigned found = fileList.at(i).find_last_of("/");
		strcpy(fName, fileList.at(i).substr(found+1).c_str());

		sprintf(buffer, "%s", fileList.at(i).c_str());
		if(stat(buffer, &info_p) == -1)
			perror("stat: ");
		if(S_ISDIR(info_p.st_mode) && 
		   !(strcmp(fName, "." ) == 0 || 
		   strcmp(fName, ".." ) == 0 )) {
			dirList->push(buffer);
		}
	}
}


int main(int argc, char**argv)
{

	int index;
	int c;
	int lnMax;
		
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
	
	//assume at least one argument
	vector<string> fileList;
	queue <string> dirList;
	lnMax = 0;
	if(optind == argc) {
		char dir_p[2];
		strcpy(dir_p, ".");
		output_ls(dir_p, & fileList, & lnMax, & dirList);
	}
	else {
		for(index = optind ; index < argc; index++){
			char buffer[1024];
			struct stat info_p;
			strcpy(buffer, argv[index]);
			if(stat(buffer, &info_p) == -1) perror("stat: ");
			if ( S_ISDIR(info_p.st_mode) )  {
				dirList.push(buffer);
			}
			else{
				int ilen;
				fileList.push_back(buffer);
				ilen= strlen(buffer);
				if(lnMax < ilen) lnMax= ilen;
			
	//		output_ls(argv[index], & fileList, & lnMax, & dirList);
			}
		}
		
			
	}
	sort(fileList.begin(), fileList.end(), compareNoCase);
	print_ls(fileList, lnMax);
	
	if(dirList.size() > 0 && Rflag == 0) {
		while(dirList.size() > 0) {
			char dir_p[1024];
			strcpy(dir_p, dirList.front().c_str());
			dirList.pop();
			lnMax = 0;
			fileList.clear();
			output_ls(dir_p, & fileList, &lnMax, & dirList);	
			sort(fileList.begin(), fileList.end(), compareNoCase);
	//		look_dir(fileList, & dirList);
			cout << endl << dir_p << ":" << endl;
			print_ls(fileList, lnMax); 
		}
	}
	if(Rflag == 1 ){
		look_dir(fileList, & dirList);
		while(dirList.size() > 0){
			char dir_p[1024];
			strcpy(dir_p, dirList.front().c_str());
			dirList.pop();
			lnMax = 0;
			fileList.clear();
			output_ls(dir_p, & fileList, &lnMax, & dirList);	
			sort(fileList.begin(), fileList.end(), compareNoCase);
			look_dir(fileList, & dirList);
			cout << endl << dir_p << ":" << endl;
			print_ls(fileList, lnMax); 
		}
	}
	return 0;
}
