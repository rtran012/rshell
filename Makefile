all: rshell ls cp
	echo "test"
	mkdir -p bin
	g++ -Wall -Werror -ansi -pedantic src/main.cpp -o bin/rshell
	
rshell:
	echo "test"
	mkdir -p bin
	g++ -Wall -Werror -ansi -pedantic src/main.cpp -o bin/rshell

ls: src/ls.cpp	
	echo "test"
	mkdir -p bin
	g++ -Wall -Werror -ansi -pedantic src/ls.cpp -o bin/ls
cp: src/cp.cpp
	echo "test"
	mkdir -p bin
	g++ -Wall -Werror -ansi -pedantic src/cp.cpp -o bin/cp
