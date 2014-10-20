#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

using namespace std;

int excute(char** argv){
	
	int status;
        int pid = fork();
         
	if(pid == -1){
                 perror("there was an error with fork().");
         }
         else if (pid == 0){
                 if(execvp(argv[0], argv) == -1){
                         perror("there was an error with execvp.");
                 }
                 exit(errno);
         }
        
	 else if (pid > 0){
		wait(&status);
		return status;
         }
	return status;
 }

int parse(char userinput[], char **argv){
	int stat;   
	int numargs=0;
    	unsigned int i = 0; 
	while(i < strlen(userinput)){
		if(userinput[i] == '#'){
			userinput[i] = '\0'; 
			break;
		}
		i++;
	}

	argv[numargs] = strtok(userinput," \t\n");

	while(argv[numargs] != NULL){
		if(strcmp(argv[numargs], ";") == 0){
			argv[numargs] = NULL;
			stat = excute(argv);
			numargs = 0;
			argv[numargs] = strtok(NULL, " \t\n");
		}

		if(strcmp(argv[numargs], "&&") == 0){
			argv[numargs] = NULL;
			stat = excute(argv);
			if(stat != 0) return numargs;
			numargs = 0;
			argv[numargs] = strtok(NULL, " \t\n");
		}

		if(strcmp(argv[numargs], "||") == 0){
			argv[numargs] = NULL;
			stat = excute(argv);
			if(stat == 0) return numargs;
			numargs = 0;
			argv[numargs] = strtok(NULL, " \t\n");
		}
		numargs++;
		argv[numargs] = strtok(NULL," \t\n");
	}
	stat = excute(argv);
    	return stat;
}
int main(){
	char username[100] = {0};
	char hostname[100] = {0};

	if(getlogin_r(username, sizeof(username)-1)){
		perror("There was an error in getlogin_r. ");
	}
	if(-1 == gethostname(hostname, sizeof(hostname)-1)){
		perror("There was an erro in gethostname. ");
	}
	for(int i = 0; i < 30 ; i++){
		if(hostname[i] == '.'){
			hostname[i] = '\0';
		}
	}

	while(1){
		cout << username << "@" << hostname << "$ ";
		char input[1024];
		char buff[1024];
		char * first, * next;
		int stat;
		bool stat1;
		cin.getline(input,1024);
		
		if(!strcmp(input, "exit")) exit(1);
		
		char ** argv;
		
		argv = new char *[50];

		if(strstr(input,";") != NULL){
			first = input;
			next = strstr(first,";");
			while(next != NULL){
				for(int i =0; i < 1024; i++) buff[i]=0;
				strncpy(buff,first,next-first);	
				stat = parse(buff, argv);
				first = next + 1;
				next = strstr(first,";");
			}

			for(int i =0; i < 1024; i++) buff[i]=0;	
			strcpy(buff, first);
			stat = parse(buff, argv);
		}

		else if(strstr(input,"&&") != NULL){
			first = input;
			next = strstr(first,"&&");
			while(next != NULL){
				for(int i =0; i < 1024; i++) buff[i]=0;
				strncpy(buff,first,next-first);	
				stat = parse(buff, argv);
				if(stat != 0) break;
				first = next+2;
				next = strstr(first,"&&");
			}

			for(int i =0; i < 1024; i++) buff[i]=0;	
			strcpy(buff, first);
			stat = parse(buff, argv);
		}
		else if(strstr(input,"||") != NULL){
			first = input;
			stat1 = true;
			next = strstr(first,"||");
			while(next != NULL && stat1){
				for(int i =0; i < 1024; i++) buff[i]=0;
				strncpy(buff,first,next-first);	
				stat = parse(buff, argv);
				if(stat == 0) stat1 = false;
				first = next+2;
				next = strstr(first,"||");
			}
			if(stat1){
				for(int i =0; i < 1024; i++) buff[i]=0;	
				strcpy(buff, first);
				stat = parse(buff, argv);
			}
		}
		else{
			stat = parse(input,argv);
		}
		
		delete[] argv;
	}
	return 0;
}
