#include <cstring>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include "Timer.h"
using namespace std;

void method1(char **args)
{
  cout << "Method 1" << endl;
  Timer t;
  double etime;
  t.start();
  ifstream ifs;
  ifs.open(args[0]);
  if(!ifs.is_open()) perror("Open");
  ofstream ofs;
  ofs.open(args[1]);
  if(!ofs.is_open()) perror("Open");
  char swap;
  while(1)
  {
    if(ifs.eof()) break;
    ifs.get(swap);
    ofs.put(swap);
  }
  t.elapsedUserTime(etime);
  cout << "Elapsed User Time: " << etime << endl;
  t.elapsedSystemTime(etime);
  cout << "Elapsed System Time: " << etime << endl;
  t.elapsedWallclockTime(etime);
  cout << "Elapsed Wallclock Time: " << etime << endl;
  ifs.close();
  ofs.close();
}
void method2(char ** args)
{
  cout << "Method 2" << endl;
  Timer t;
  double etime;
  t.start();
  int fi=open(args[0],O_RDONLY);
  if(fi==-1) perror("Open");
  int fo=open(args[1],O_RDWR | O_CREAT,0666);
  if(fo==-1) perror("Open");
  char * pswap=new char;
  int rsize,wsize;
  while(1)
  {
    rsize=read(fi,pswap,1);
    if(rsize==0) break;
    else if (rsize==-1)
    {
      perror("Read");
      exit(1);
    }
    wsize=write(fo,pswap,1);
    if(wsize==-1)
    {
      perror("Write");
      exit(1);
    }
  }
  t.elapsedUserTime(etime);
  cout << "Elapsed User Time: " << etime << endl;
  t.elapsedSystemTime(etime);
  cout << "Elapsed System Time: " << etime << endl;
  t.elapsedWallclockTime(etime);
  cout << "Elapsed Wallclock Time: " << etime << endl;
  delete pswap;
  if(close(fi)==-1) perror("Close");
  if(close(fo)==-1) perror("Close");
}
void method3(char ** args)
{
  cout << "Method 3" << endl;
  Timer t;
  double etime;
  t.start();
  int fi=open(args[0],O_RDONLY);
  if(fi==-1) perror("Open");
  int fo=open(args[1],O_RDWR | O_CREAT,0666);
  if(fo==-1) perror("Open");
  char * pswap=new char[BUFSIZ];
  int rsize,wsize;
  while(1)
  {
    rsize=read(fi,pswap,BUFSIZ);
    if(rsize==0) break;
    else if (rsize==-1)
    {
      perror("Read");
      exit(1);
    }
    wsize=write(fo,pswap,BUFSIZ);
    if(wsize==-1)
    {
      perror("Write");
      exit(1);
    }
  }
  t.elapsedUserTime(etime);
  cout << "Elapsed User Time: " << etime << endl;
  t.elapsedSystemTime(etime);
  cout << "Elapsed System Time: " << etime << endl;
  t.elapsedWallclockTime(etime);
  cout << "Elapsed Wallclock Time: " << etime << endl;
  delete[] pswap;
  if(close(fi)==-1) perror("Close");
  if(close(fo)==-1) perror("Close");
}

bool lp(char ** argv, char ** args, int c)
{
  bool doall=false;
  int curr=0;
  for(int i=1;i<c;i++)
  {
    if(argv[i][0]=='-')
    {
      if(argv[i][1]=='a')
      {
        doall=true;
        if(i==(c-1)) return true;
        else i++;
      }
      else
      {
        cerr << "Invalid Flag. Try -a." << endl;
        exit(1);
      }
    }
    args[curr]=new char[strlen(argv[i])+1];
    strcpy(args[curr],argv[i]);
    curr++;
  }
  return doall;
}

int main(int c, char ** argv)
{
  if(c<3 or c>4)
  {
    cerr << "Incorrect number of args" << endl;
    exit(1);
  }
  char ** args=new char*[3];
  for(int i=0;i<3;i++) args[i]=0;
  bool doall=lp(argv, args, c);
  if(args[1]==0)
  {
    cerr << "Dest File not specified" << endl;
    exit(1);
  }
  struct stat* buff=new struct stat;
  int test=stat(args[1],buff);
  if(test==0)
  {
    cerr << "Dest File already exists" << endl;
    exit(1);
  }
  if(doall)
  {
    method1(args);
    method2(args);
  }
  method3(args);
  return 0;
}
