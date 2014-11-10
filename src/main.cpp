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
	char ** cmd;
	char * next, * first;
	int ncmd = 0;
//	cout << "userinput " << userinput << endl;	
	//
	cmd = new char *[50];
	first = userinput;
	next = strstr(first, "|");
	cmd[0] = first;
	ncmd++;
	while(next != NULL){
		cmd[ncmd] = next+1;
		*next = 0 ;
		first = next +1;
		next = strstr(first, "|");	
		ncmd++;
	}	
	cout << "ncmd " << ncmd << endl;
	for(int i = 0; i < ncmd; i++){
		cout<< i << " cmd[i] " << cmd [i] << endl;
	}


        /* implement the piping function */
       // int  nargs;
        int  pid, status;
        int  icur_pipe;
        int  *fd;
        //int  ipipe=0;
        int  n_ends = 0;

        fd = (int *) malloc( (ncmd-1)*2 * sizeof(int) );

        /* Start the first child */
        icur_pipe = 0;

        for (int i=0; i < ncmd - 1; i++) {
            pipe( &fd[i*2] );
            cout << " Start the pipe number " << icur_pipe << endl;
        }
        n_ends = (ncmd - 1)*2;

        switch ( pid = fork() ){
            case 0: /* child */
                //dup2( fd[icur_pipe*2+1], 1 );
                //close( fd[icur_pipe*2] );
                for ( int i=0; i <  n_ends; i++) {
                    if ( i == 1 ) { 
                       dup2 ( fd[i], 1);
                    } else {
                       close ( fd[i] );
                    }
                }
                gen_cmd( cmd[icur_pipe], argv);
                if ( execvp ( argv[0], argv ) == -1 ) {
                     perror (" cmd[icur_pipe] execvp ");
                }
                exit(errno);
            case -1: 
                perror("fork");
                exit(1);
        }

        cout << "Start the child process " << pid << endl;
        /* Loop until the last command */
        for (int j= 1; j < ncmd - 1 ; j++ ){
            icur_pipe ++;
            //pipe( &fd[ icur_pipe * 2 ] );
            cout << " Start the pipe number " << icur_pipe << endl;
            cout << " Start the command line " << cmd[icur_pipe] << endl;

            switch ( pid = fork() ) {
                case 0:   /* another child */
                 //   dup2 ( fd[ (icur_pipe-1) * 2],       0 );
                 //   //close ( fd[ (icur_pipe-1) * 2 + 1] );
                 //   dup2 ( fd[ (icur_pipe) * 2 + 1],   1 );
                    for ( int i = 0 ; i < n_ends; i++) {
                        if ( i == (j-1)*2 ) {
                            dup2 ( fd[ (j - 1) * 2 ], 0 );
                        } else if ( i == j*2 + 1) {
                            dup2 ( fd[ j * 2 + 1 ], 1 );
                        } else {
                           close ( fd[i] );
                        }
                    }
                    gen_cmd( cmd[ icur_pipe], argv);
                    if ( execvp ( argv[0], argv ) == -1 ) {
                         perror (" cmd[ipipe] execvp ");
                    }
                    exit(errno);
                case -1:
                    perror("fork");
                    exit(1);
            }
            cout << "Start the child process " << pid << endl;
      
        }
        /* Start the last child */
        icur_pipe++;
        cout << " Start the last command " << cmd[icur_pipe] << endl;
        switch ( pid = fork() ) {
            case 0: /* 2nd child */
                // dup2( fd[(icur_pipe-1) * 2], 0 );
                // close( fd[(icur_pipe-1) *2 + 1] );  /* this child don't need the other end */
                for (int i = 0 ; i < n_ends; i++){
                    if ( i == n_ends - 2 ) {
                       dup2 ( fd[ n_ends - 2], 0 );
                    } else {
                       close ( fd[ i ]);
                    }
                }
                gen_cmd( cmd[ icur_pipe ], argv );
                if ( execvp ( argv[0], argv ) == -1 ) {
                     perror ( " cmd[icur_pipe] execvp ");
                }
                exit(errno);
            case -1:
                perror("fork");
                exit(1);
        }
        cout << "Start the last child process " << pid << endl;

        /* Still parent */
//       for ( int j = 0; j < ncmd - 1 ; j++ ) {
//           cout << " close the pipe " << j << endl;
//           close( fd[j*2]   );
//           close( fd[j*2+1] );
//        }
        cout << "Try to close fd[0] and fd[" << ( ncmd-2)*2+1  << endl;

//        close ( fd[0] );
//        close ( fd[(ncmd -2 )* 2 +1 ] );
        for (int i = 0 ; i < n_ends ; i++) {
            close ( fd[i]);
        }

        /* wait for all child */
        cout << "after two close functions, pid= " << pid << endl;
        while ( ( pid = wait(&status)) != -1 ) 
             fprintf(stderr, "process %d exists with %d\n", pid, WEXITSTATUS(status));
      
      
        cout << "Just before ending the function parse2 "<< endl;
	return ncmd;
}
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
