#include <iostream>
#include <ctype.h>
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
/*void lsl(struct dirent currFile, & fileList){
	
					struct stat buff;
					int status;
					status = stat(currFile->d_name , & buff);
					if(status == -1) perror("stat: ");
					else{
						
						char timeBuff [50];
						strftime(timeBuff, 50, "%b %d %R", buff.st_mtime);
						cout << "file type " ;
						(buff.st_mode & S_IRUSR) ? cout << "r" : cout << "-";
						(buff.st_mode & S_IWUSR) ? cout << "w" : cout << "-"; 
						(buff.st_mode & S_IXUSR) ? cout << "x" : cout << "-";
						(buff.st_mode & S_IRGRP) ? cout << "r" : cout << "-";
						(buff.st_mode & S_IWGRP) ? cout << "w" : cout << "-";
						(buff.st_mode & S_IXGRP) ? cout << "x" : cout << "-";
						(buff.st_mode & S_IROTH) ? cout << "r" : cout << "-";
						(buff.st_mode & S_IWOTH) ? cout << "w" : cout << "-";
						(buff.st_mode & S_IXOTH) ? cout << "x" : cout << "-";
						cout << " " << buff.st_nlink << " " <<
						buff.st_uid << " " << 
						buff.st_gid << " " << 
						buff.st_size << " " << 
						//buff.st_mtime << " " << 
						cout << timeBuff << " " << 
						currFile->d_name <<  endl;
					}
	}*/

void mode_to_letters( int mode, char str[] )
{
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


char *uid_to_name( uid_t uid )
{
	//struct	passwd *getpwuid(), *pw_ptr;
	struct	passwd *pw_ptr;
	static  char numstr[10];

	if ( ( pw_ptr = getpwuid( uid ) ) == NULL ){
		sprintf(numstr,"%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name ;
}


char *gid_to_name( gid_t gid )
{
	struct group *grp_ptr;
	static  char numstr[10];

	if ( ( grp_ptr = getgrgid(gid) ) == NULL ){
		sprintf(numstr,"%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
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
	//	cout << "ncolum= " << ncolum << " nlines= " << nlines << " maxLength= " << maxLength << endl;
		for(unsigned int i = 0 ; i < nlines ; i++){
			for(unsigned int k = 0 ; k < ncolum; k++){
				if(i+nlines*k < fileList.size()){
					unsigned found = fileList.at(i+nlines*k).find_last_of("/");
					strcpy(format_file, fileList.at(i+nlines*k).substr(found+1).c_str());
					printf("%s", format_file);
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
		for(unsigned int i = 0; i < fileList.size(); i ++){
			struct stat info_p;
			if(lstat(fileList.at(i).c_str() , &info_p) == -1)
				perror("stat: ");
			else{
			//	char	*uid_to_name(), *ctime(), *gid_to_name(), *filemode();
			//	void	mode_to_letters();
       		 		char    modestr[11];

				mode_to_letters( info_p.st_mode, modestr );
	
				printf( "%s"    , modestr );
				printf( "%4d "  , (int) info_p.st_nlink);	
				printf( "%-8s " , uid_to_name(info_p.st_uid) );
				printf( "%-8s " , gid_to_name(info_p.st_gid) );
				printf( "%8ld " , (long)info_p.st_size);
				printf( "%.12s ", 4+ctime(&info_p.st_mtime));
				unsigned found = fileList.at(i).find_last_of("/");
				printf( "%s\n"  , fileList.at(i).substr(found+1).c_str());
			} 
		}
	}
		
}
void output_ls(char * dirName, vector<string> * fileList, int * lengthMax ){
	
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
				cout << "currFile " << currFile->d_name << endl;
				sprintf(buffer, "%s/%s", dirName, currFile->d_name);
				fileList->push_back(buffer);
				ilen= strlen(currFile->d_name);
				if(*lengthMax < ilen) *lengthMax= ilen;
			}
		}
		closedir(currPath);
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
	
	cout << "a =" << aflag << endl;
	cout << "l =" << lflag << endl;
	cout << "R =" << Rflag << endl;
	for(index = optind ; index < argc; index++)
		cout<< index << " Non-option argument " << argv[index] << endl ;	
	//assume at least one argument
	vector<string> fileList;
	lnMax = 0;
	if(optind == argc) {
		char dir_p[2];
		strcpy(dir_p, ".");
		output_ls(dir_p, & fileList, & lnMax);
	}
	else {
		for(index = optind ; index < argc; index++)
			output_ls(argv[index], & fileList, & lnMax);
			
	}
	sort(fileList.begin(), fileList.end(), compareNoCase);

	print_ls(fileList, lnMax);
//	for(unsigned int i = 0 ; i < fileList.size() ; i++){
//		cout << "fileList : " << fileList.at(i) << endl;
//	} 
	return 0;
}








