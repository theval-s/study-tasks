#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h> //for opendir & readdir()
#include <sys/types.h> //for opendir()

//var: 2 & even
//opendir/readdir with recursion
//searching for files containing string of utf-8 bytes (input)
//example of launching ./lab11vsaN32471 /home "лавандовый раф"

//options 
//-v --version
//-h --help
//environment variables: LAB11DEBUG - enabling or disabling debug info

/*	TO-DO
*makefile - done (i think so)
*directory-walking using opendir/readdir with recursion
*file reading (main algorithm)
*options (getopt_long)
*catching errors
*environment variables (getenv, fprintf(stderr, ...))
*/

void walking(char *dir){
	DIR *d = opendir(dir);
	if (d == NULL){
		printf("Failed to opendir() %s\n", dir);
		return;
	}
}

int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Usage: %s <dir>\n\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	walking(argv[1]);
	return 0;
}

