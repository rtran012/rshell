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

int gen_cmd( char userinput[], char **argv){
        int numargs=0;
        argv[numargs] = strtok(userinput," \t\n");
        while (argv[numargs] != NULL ){
             numargs++;
             argv[numargs] = strtok(NULL, " \t\n");
        }
        argv[numargs+1] = NULL;  /* ending by null character */
        return numargs; 
}

int parse2(char userinput [], char **argv){
        //int  istat;
	char ** cmd;
	char * first;
	int ncmd = 0;
        int icmd;

	cout << "userinput " << userinput << endl;	

        char op_cmd[50];

	cmd = new char *[50];

        //cout << "DEBUG userinput = " << userinput << endl;
        //cout << " strlen = " << strlen(userinput) << endl;
        first = userinput;
        icmd  = 0;
	int nlen = strlen(userinput);
        for( int i=0; i < nlen ; i++) {
            if(strncmp (&userinput[i], "|", 1 ) == 0 ) {
                 cmd[icmd] = first;
                 first = &userinput[i+1];
                 op_cmd[icmd] = '|';
                 userinput[i] = 0;
                 icmd++;
            } else if ( strncmp( &userinput[i], "<", 1 ) == 0 ) {
                 cmd[icmd] = first;
                 first = &userinput[i+1];
                 op_cmd[icmd] = '<';
                 userinput[i] = 0;
                 icmd++;
            } else if ( strncmp( &userinput[i], ">", 1 ) == 0 ) {
                 cmd[icmd] = first;
                 first = &userinput[i+1];
                 op_cmd[icmd] = '>';
                 userinput[i] = 0;
                 icmd++;
            }
        }
        cmd[icmd] = first;
        op_cmd[icmd] = 0;
	ncmd = icmd;
	

        cout << "Found there are " << icmd << " operator "<<endl;
        for (int i=0; i < icmd ; i++){
           cout << i << " cmd= "<< cmd[i] << endl;
           cout << "   the operator is " << op_cmd[i] << endl;
        }
        cout << icmd  << " cmd= "<< cmd[ icmd] << endl;
	

    //    if (icmd > 0 ) return ncmd;

/*	first = userinput;
	next = strstr(first, "|");
        if ( next == NULL ) {

           gen_cmd( userinput, argv);
           istat = excute(argv);

        } else {

	cmd[0] = first;
	ncmd++;
	while(next != NULL){
		cmd[ncmd] = next+1;
		*next = 0 ;
		first = next +1;
		next = strstr(first, "|");	
		ncmd++;
	}	

	//cout << "ncmd " << ncmd << endl;
	//for(int i = 0; i < ncmd; i++){
        //    cout<< i << " cmd[i] " << cmd [i] << endl;
        //}
*/

        /* implement the piping function */
        //int  nargs;
//        int  pid, status;
	int pid;
	int status;
	int newfd[2];
//        int  icur_pipe;
        int  fd[2];
	if(ncmd == 0){
		gen_cmd(cmd[0], argv);
		status = excute(argv);
		return status;
	}
	else{	//taking care of piping
		pipe(fd);
		switch( pid = fork()){
			case 0: 
				if(strncmp(& op_cmd[0], "|", 1) == 0){
					dup2( fd[1], 1);
					close( fd[0] );
				}
				gen_cmd(cmd[0], argv);
				if( execvp( argv[0], argv) == -1) {
					perror("execvp " );
				}
				exit(errno);	
			case -1:
				perror("fork ");
				exit(1);
		}	
		for(int j= 1; j < ncmd - 1; j++){
			pipe(newfd);
			switch( pid = fork()){
				case 0:
					if(strncmp( &op_cmd[j], "|", 1) == 0){
						dup2( fd[0], 0);
						close( fd[1] );
						dup2( newfd[1], 1);
						close( newfd[0]); 
					}
					gen_cmd(cmd[j], argv);
					if( execvp( argv[0], argv) == -1){
						perror("execvp " );
					}
					exit(errno);
				case -1:
					perror("fork ");
					exit(1);
			}
			close( fd[0]);
			close( fd[1]);
			fd[0] = newfd[0];
			fd[1] = newfd[1];
		}
		switch( pid = fork()) {
			case 0:
				dup2( fd[0], 0);
				close( fd[1] );
				gen_cmd(cmd[ncmd], argv);
				if( execvp(argv[0], argv) == -1){
					perror("execvp ");
				}
				exit(errno);
			case -1:
				perror("fork ");
				exit(1);
		}
		close( fd[0] );
		close( fd[1] );
		while((pid = wait(&status)) != -1);
	}
	if(ncmd == 0)
		return ncmd;
	return ncmd;
}

int parse(char userinput[], char **argv){
	int stat;   
	int numargs=0;
	argv[numargs] = strtok(userinput," \t\n");

	while(argv[numargs] != NULL){
/*		if(strcmp(argv[numargs], ";") == 0){
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
		}*/
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
		
		//how to handal comments
		unsigned int i = 0; 
		while(i < strlen(input)){
			if(input[i] == '#'){
				input[i] = '\0'; 
				break;
			}
			i++;
		}

		//how to exit program (must be on its own line)
		if(!strcmp(input, "exit")) exit(1);
		
		char ** argv;
		
		argv = new char *[50];

		//taking care of ; 
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

		// taking care of && only after ; is taken care of
		else if(strstr(input,"&&") != NULL){
			first = input;
			stat1 = true;
			next = strstr(first,"&&");
			while(next != NULL && stat1){
				for(int i =0; i < 1024; i++) buff[i]=0;
				strncpy(buff,first,next-first);	
				stat = parse(buff, argv);
				if(stat != 0) stat1 = false;
				first = next+2;
				next = strstr(first,"&&");
			}
			if(stat1){
				for(int i =0; i < 1024; i++) buff[i]=0;	
				strcpy(buff, first);
				stat = parse(buff, argv);
			}
		}

		// takin care of || only after && and ; is taken care of
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
		
		// single argument 
		else{
			stat = parse2(input,argv);
		}
		
		delete[] argv;
	}
	return 0;
}
