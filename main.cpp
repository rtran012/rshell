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

void excute(char** argv){
         int pid = fork();
         if(pid == -1){
                 perror("there was an error with fork().");
         }
         else if (pid == 0){
                 if(execvp(argv[0], argv) == -1){
                         perror("there was an error with execvp.");
                 }
                 exit(1);
         }
         else if (pid > 0){
                 if(-1 == wait(0)){
                         perror("there was an error with wait().");
                 }
         }
 }

int parse(char userinput[], char **argv){
	   
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
		
  /*  	while(argv[numargs] != NULL){
        	if(!strcmp(argv[numargs],"#")){
            		argv[numargs] = NULL;
            		if(numargs > 0) numargs--;
            		break;
        	}   
		char *copy = argv[numargs];
		int length = strlen(argv[numargs]);
		if(copy[0] == '#'){
			argv[numargs] = NULL;
			if(numargs > 0) numargs--;
			break;
		}
		if(copy[length-1] == '#'){
			copy[length-1] = '\0';
			break;
		}
		bool shouldbreak=false;
		for(int i=0; i<length; i++) {
			if(copy[i] == '#'){
				copy[i] = '\0';
				argv[numargs] =copy;
				shouldbreak=true;
			}
		}
		if(shouldbreak) break;

        	numargs++;
		argv[numargs] = strtok(NULL, " \t\n");
    	}*/
	while(argv[numargs] != NULL){
		if(strcmp(argv[numargs], ";")==0){
			argv[numargs] = NULL ;
			excute(argv);
                         numargs = 0;
                         argv[numargs]=strtok(NULL," \t\n");
                 }

		numargs++;
		argv[numargs] = strtok(NULL," \t\n");
//		if(strcmp(argv[numargs], ";")==0){
//			excute(argv);
//			numargs = 0;
//			argv[numargs]=strtok(NULL," \t\n");
//		}
	}
	excute(argv);
	cout << "arguments " << endl;
	for (int i = 0 ; i < numargs; i ++){
		cout << argv[i] << endl;
	}
    	return numargs;
}
/*
bool background(int numargs, char** argv, bool emptyinput){
	bool backgroundProc;
	if(numargs > 0) numargs --;
	if(!emptyinput && !strcmp(argv[numargs],"&")){
        	backgroundProc = true;
        	argv[numargs] = NULL;
    	}
	else if(!emptyinput){
        	char* copy = argv[numargs];
        	int arglen = strlen(argv[numargs]) - 1;
        	if(copy[arglen] == '&'){
            		backgroundProc=true;
			copy[arglen] = '\0';
            		argv[numargs] = copy;
        	}
    	}
    	return backgroundProc;
}

void excute(char** argv){
	int pid = fork();
	if(pid == -1){
		perror("there was an error with fork().");
	}
	else if (pid == 0){
		if(execvp(argv[0], argv) == -1){
			perror("there was an error with execvp.");
		}
		exit(1);
	}
	else if (pid > 0){
		if(-1 == wait(0)){
			perror("there was an error with wait().");
		}
	}
}
*/
int main(){

	while(1){
		cout << "$ ";
		char input[1024];
		cin.getline(input,1024);
		if(!strcmp(input, "exit")) exit(1);
		/*bool varAnd = false;
		if(strcmp(input, "&&")) varAnd=true;
		bool varOr = false;
		if(strcmp(input, "||")) varOr = true;
		bool varSem = false;
		if(strcmp(input, ";")) varSem = true;
		*/char ** argv;
		argv = new char *[50];

//		bool emptyInput = false;
//		if(!strcmp(input, "")) emptyInput = true;

			parse(input, argv);
		/*
		int j = 0;
	//	int loc=0;
		for(j =0; j < i; j++){
			if(strcmp(argv[j],";")) {
	//			char ** new_argv;
	//			new_argv = new char *[50];
	//			for(int k = 0 ; k < j; k++){
	//				new_argv[k] = argv[j];
	//			}
				 
				excute(argv);
				delete[] new_argv;
	//			loc = j+1;
			}
		}	
		
//		bool backgroundProc = false;
//		backgroundProc = background (i, argv, emptyInput);
		//excute(argv);		*/
		delete[] argv;
	}
	return 0;
}
