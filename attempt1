#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv){
	cout << "hi" << endl;	
	int fail = execvp(argv[0], argv);
	cout << "blab" << endl;	
	if (fail == -1){
		cout << "should not happen" << endl;
		return 1;
	}
	 return 0;


}
