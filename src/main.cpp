#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

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

/* This routine will execute the command with redirection option */
int scan_cmd_redirect( char userinput[], char **argv) {
//        int numargs = 0;
	int append_flag = 0;
        char *filein, *fileout;
        char *curr_pt_in, *curr_pt_out;
        char temp_in[1024];
        char temp_out[1024];

        //cout << " Input: userinput = " << userinput << endl;

        // Search for input redirection  information
        filein = NULL; 
        if ( (curr_pt_in = strstr(userinput,"<")) != NULL ) {
             strcpy(temp_in, curr_pt_in+1);
             filein = strtok(temp_in," \n\t><");
             if ( filein == NULL ) {
//                  numargs = -1;
                  cout << "Can not find <  in userinput " << userinput << endl;
            }
        } 

        // Search for output redirection information
        fileout = NULL;
	if ( (curr_pt_out = strstr(userinput, ">>")) != NULL){
	    strcpy(temp_out, curr_pt_out+2);
	    fileout = strtok(temp_out," \n\t><");
	    if(fileout == NULL){
		cerr << "Can not find >> in userinput " << userinput << endl;
	    }
	    append_flag = 1;
	}
        else if ( (curr_pt_out = strstr(userinput,">")) != NULL){
            strcpy(temp_out, curr_pt_out+1);
            fileout = strtok(temp_out," \n\t><");
            if (fileout == NULL) {
    //             numargs = -1;
                 cerr << "Can not find > in userinput " << userinput << endl;
            }
        }


        // mark end of the current command
        if ( curr_pt_in  != NULL ) *curr_pt_in  = 0;
        if ( curr_pt_out != NULL ) *curr_pt_out = 0;

        //if (filein  != NULL ) cout <<  " Redirect input: " << filein << endl;
        //if (fileout != NULL ) cout <<  " Redirect output: " << fileout << endl;


//        int status;
        int new_stdin, new_stdout;

        new_stdin = -1; 
        new_stdout = -1;

        // Open both input/output redirection if exists
        if ( filein != NULL ) 
                new_stdin = open (filein, O_RDONLY);
        if ( fileout != NULL && append_flag == 0 ){
                new_stdout = open(fileout, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO);
	}
	else if( fileout != NULL && append_flag == 1){
                new_stdout = open(fileout, O_WRONLY | O_APPEND , S_IRWXU|S_IRWXG|S_IRWXO);
			
	}
        // Replacing input redirection
        if ( new_stdin != -1 ) {
                close ( 0 );
                dup( new_stdin );
                close ( new_stdin );
        }

        // Replacing output redirection
        if ( new_stdout != -1 ) {
                close ( 1 );
                dup ( new_stdout );
                close ( new_stdout );
        }

        int nargs;
        //int i;
        nargs = gen_cmd(userinput, argv);

       return nargs;      
}

int parse(char userinput [], char **argv){
        //int  istat;
	char ** cmd;
	char * first;
	int ncmd = 0;
        int icmd;
	int background_flag;

//	cout << "userinput " << userinput << endl;	

        //char op_cmd[50];

	cmd = new char *[50];

        //cout << "DEBUG userinput = " << userinput << endl;
        //cout << " strlen = " << strlen(userinput) << endl;
	//sreach for option to execute in background
	char * cur_pt;
	background_flag = 0;
	if((cur_pt = strstr(userinput, "&")) != NULL){
	    background_flag = 1;
	    *cur_pt = ' ';
	}

        first = userinput;
        icmd  = 0;
	int nlen = strlen(userinput);
        for( int i=0; i < nlen ; i++) {
            if(strncmp (&userinput[i], "|", 1 ) == 0 ) {
                 cmd[icmd] = first;
                 first = &userinput[i+1];
        //         op_cmd[icmd] = '|';
                 userinput[i] = 0;
                 icmd++;
            }
        }
        cmd[icmd] = first;
     //   op_cmd[icmd] = 0;
	ncmd = icmd;
	

        //cout << "Found there are " << ncmd << " operator "<<endl;
        //for (int i=0; i < ncmd ; i++){
        //   cout << i << " cmd= "<< cmd[i] << endl;
        //   cout << "   the operator is " << op_cmd[i] << endl;
        //}
        //cout << icmd  << " cmd= "<< cmd[ ncmd] << endl;
	

    //    if (icmd > 0 ) return ncmd;


        /* implement the piping function */
	int pid;
	int status;
	int newfd[2];
        int  fd[2];
	status = 0;

	if(ncmd == 0){

                pid = fork ();
                if ( pid == -1 ) {
                      perror("There was an error with fork().");
                } else if ( pid == 0 ) {
                      scan_cmd_redirect ( cmd[0], argv );
                      if ( execvp ( argv[0], argv ) == -1 ) 
                                  perror ("there was an error with execvp." );
                      exit(errno);
                } else if ( pid > 0) {
		      if(background_flag == 0){
                      	  wait( &status );
                          return status;
		      }
		      else{
			  return 0;
		      }
                }
            //    return 0;
                       
	}
	else{	//taking care of piping
		pipe(fd);
		switch( pid = fork()){
			case 0: 
                                scan_cmd_redirect ( cmd[0], argv );

				dup2( fd[1], 1);
				close( fd[0] );

                                if ( execvp ( argv[0], argv ) == -1 )
                                      perror ( "execvp ");
                                exit ( errno );
			case -1:
				perror("fork ");
				exit(1);
		}	
		for(int j= 1; j < ncmd ; j++){
			pipe(newfd);
			switch( pid = fork()){
				case 0:
                                        scan_cmd_redirect ( cmd[j], argv );

					dup2( fd[0], 0);
					close( fd[1] );
					dup2( newfd[1], 1);
					close( newfd[0]); 

                                        if ( execvp ( argv[0], argv ) == -1 )
                                              perror ( "exevp ");
                                        exit ( errno );
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
                                scan_cmd_redirect ( cmd[ncmd], argv );

				dup2( fd[0], 0);
				close( fd[1] );

                                if ( execvp ( argv[0], argv ) == -1 )
                                      perror ( "exevp ");
                                exit ( errno );
			case -1:
				perror("fork ");
				exit(1);
		}
		close( fd[0] );
		close( fd[1] );
		if(background_flag == 0){
			while((pid = wait(&status)) != -1);
			return status;
		}
		else{
			return 0;
		}
	}

	return status;
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
			stat = parse(input,argv);
		}
		
		delete[] argv;
	}
	return 0;
}
