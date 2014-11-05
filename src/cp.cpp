#include <sys/types.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sys/stat.h>
#include "Timer.h"
#include <errno.h>

using namespace std;

void optionOne(char ** argv) 
{
	ifstream in;
	ofstream out;

	in.open(argv[1]);
	out.open(argv[2]);

	while (!in.eof())         
	{		
		char c = in.get();      
		if (in.good())
		out.put(c);
	}	

	in.close();
	out.close();
}

void optionTwo(char** argv)
{
  int fd=open(argv[1], O_RDONLY);
  int fd2=open(argv[2],O_RDWR | O_CREAT);
//  fd2 & S_IRUSR & S_IWUSR;

  if (fd == -1) {
    perror("open 1 failed");
  }
  if (fd2 == -1) {
    perror("open 2 failed");
  }

  char buf[1];
  while (read(fd, buf, 1) != 0) {
    write(fd2, buf, 1);
  }

  if (close(fd) == -1) perror("close failed");
  if (close(fd2) == -1) perror("close failed");

}

void optionThree(char** argv)
{
  int fd=open(argv[1], O_RDONLY);
  int fd2=open(argv[2],O_RDWR | O_CREAT);
  //fd2 & S_IRUSR & S_IWUSR;
  
  if (fd == -1) {
    perror("open 1 failed");
  }
  if (fd2 == -1) {
    perror("open 2 failed");
  }

  char* buf[BUFSIZ];
  while (read(fd, buf, BUFSIZ) != 0);
 {
    write(fd2, buf, BUFSIZ );
  }

  if (close(fd) == -1) perror("close failed");
  if (close(fd2) == -1) perror("close failed");

}

int main(int argc, char** argv){
	fstream file(argv[2]);
		if (file)
		{
			cout << "File already exists!" << endl;
			return 1;

		}	
	
Timer t;
  Timer t2;
  Timer t3;
  Timer t4;

  double eTime = 0.0;
  double eTime2 = 0.0;
  double eTime3 = 0.0;
  double eTime4 = 0.0;

  double wTime = 0.0;
  double wTime2 = 0.0;
  double wTime3 = 0.0;
  double wTime4 = 0.0;

  double uTime = 0.0;
  double uTime2 = 0.0;
  double uTime3 = 0.0;
  double uTime4 = 0.0;


  //if ((!is_dir(argv[1])) && (!is_dir(argv[2]))) {

     if (argc == 3) {
        cout << "Fastest: option 3" << endl;

        t.start();

        optionThree(argv);

        t.elapsedUserTime(eTime);
        t.elapsedWallclockTime(wTime);
        t.elapsedSystemTime(uTime);

        cout << "Elapsed User Time: " << eTime << endl;
        cout << "wallclock: " << wTime << endl;
        cout << "Elapsed system time : " << uTime << endl;


     }

     if (strcmp(argv[3]) == "time") {

	cout << "option 1" << endl;

        t3.start();

        optionThree(argv);

        t3.elapsedUserTime(eTime3);
        t3.elapsedWallclockTime(wTime3);
        t3.elapsedSystemTime(uTime3);

        cout << "Elapsed User Time: " << eTime3 << endl;
        cout << "wallclock: " << wTime3 << endl;
        cout << "Elapsed system time : " << uTime3 << endl;

        cout << "" << endl;

        cout << "option 2" << endl;

        t2.start();

        optionTwo(argv);

        t2.elapsedUserTime(eTime2);
        t2.elapsedWallclockTime(wTime2);
        t2.elapsedSystemTime(uTime2);

        cout << "Elapsed User Time: " << eTime2 << endl;
        cout << "wallclock: " << wTime2 << endl;
        cout << "Elapsed system time : " << uTime2 << endl;

        cout << "" << endl;
        cout << "option 3" << endl;

        t4.start();

        optionThree(argv);

        t4.elapsedUserTime(eTime4);
        t4.elapsedWallclockTime(wTime4);
        t4.elapsedSystemTime(uTime4);

        cout << "Elapsed User Time: " << eTime4 << endl;
        cout << "wallclock: " << wTime4 << endl;
        cout << "Elapsed system time : " << uTime4 << endl;


     }
//}

//	option3(argv);
	
}
