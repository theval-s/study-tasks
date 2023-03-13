#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//var: 2 & even
//opendir/readdir with recursion
//searching for files containing string of utf-8 bytes (input)
//example of launching ./lab11vsaN32471 /home "лавандовый раф"

//options 
//-v --version
//-h --help
//environment variables: LAB11DEBUG - enabling or disabling debug info

/*	TO-DO
*makefile ----"make pack"
*directory-walking using opendir/readdir with recursion
*file reading (main algorithm)
*options (getopt_long)
*environment variables (getenv)
*/

int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Usage: %s <dir>\n\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	return 0;
}

