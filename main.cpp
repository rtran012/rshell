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
		if(strcmp(argv[numargs], ";")==0){
			argv[numargs] = NULL ;
			stat = excute(argv);
			cout << "stat " << stat << endl;
                         numargs = 0;
                         argv[numargs]=strtok(NULL," \t\n");
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
//	cout << "stat " << stat << endl;
    	return numargs;
}
int main(){

	while(1){
		cout << "$ ";
		char input[1024];
		cin.getline(input,1024);
		
		if(!strcmp(input, "exit")) exit(1);
		
		char ** argv;
		
		argv = new char *[50];


		parse(input, argv);
		
		delete[] argv;
	}
	return 0;
}
