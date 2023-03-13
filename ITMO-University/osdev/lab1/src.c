#include <dirent.h>  //for opendir & readdir()
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for chdir
#include <sys/types.h>  //for opendir() 

// var: 2 & even
// opendir/readdir with recursion
// searching for files containing string of utf-8 bytes (input)
// example of launching: ./lab11vsaN32471 /home "лавандовый раф"

/*	TO-DO
 *makefile - done (i think so)
 *directory-walking using opendir/readdir with recursion - done
 *file reading - done
 *options (getopt_long)
 *catching errors - kinda? not everywhere
 *environment variables (getenv, fprintf(stderr, ...))
*/

// options
//-v --version
//-h --help
// environment variables: LAB11DEBUG - enabling or disabling debug info

void walking(int level, char *dir, char *desired_string) {
  DIR *d = opendir(dir);
  if (d == NULL) {
    printf("Failed to opendir() %s\n", dir);
    return;
  }
  for (;;) {
    errno = 0;
    struct dirent *p = readdir(d);
    /*
dirent{ d_ino - inode number, d_off - telldir, d_reclen - length,
d_type - type of file, d_name[256] - filename};
d_type - DT_DIR - directory, DT_LNK - symbolic link, DT_REG - file, etc
    */
    if (p == NULL) {
      if (errno != 0) continue;
      // man readdir - EOF returned on both EOF and errors
      else
        break;
    }
    
    
    // strcmp returns 0 if strings are equal
    // so we check that none of them is 0
    if (strcmp(p->d_name, ".") && strcmp(p->d_name, "..")) {
    	//for(int i = 0; i < level; i++) printf("--"); //printing levels
      //printf("%s - type %d\n", p->d_name, p->d_type);
      if (p->d_type == DT_DIR) {
        char newdir[4096];
        sprintf(newdir, "%s/%s", dir, p->d_name); //
        // new directory for opendir() and readdir()
        walking(level+1, newdir, desired_string);
      } else {
      	short found_flag = 0;
      	chdir(dir);
      	FILE *fp = fopen(p->d_name, "r");
      	if(!fp){
      		printf("error opening file %s!\n", p->d_name);
      		exit(1);
      	} else { 
      		char *buf = NULL;
      		size_t buf_len = 0;
      		ssize_t read;
      		while((read = getline(&buf, &buf_len, fp)) != -1){
      			printf("%s", buf);
      			if(strstr(buf, desired_string) != NULL) found_flag = 1; 
      			}
      		}
      	fclose(fp);
      	if(found_flag) printf("found file: %s \n", p->d_name);
      }
    }
  }

  closedir(d);
  return;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <dir> <search_query>\n\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  walking(0, argv[1], argv[2]);
  return 0;
}

