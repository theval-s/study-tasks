#define _GNU_SOURCE  // for get_current_dir_name
#include <stdlib.h>
#include <dirent.h>  //for opendir & readdir()
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>  //for opendir()
#include <unistd.h>     // for chdir

// var: 2 & even
// opendir/readdir with recursion
// searching for files containing string of utf-8 bytes (input)
// example of launching: ./lab11vsaN32471 /home "лавандовый раф"

/*	TO-DO
 *makefile - done (i think so)
 *directory-walking using opendir/readdir with recursion - done
 *file reading - done
 *options (getopt_long) - done
 *catching errors - kinda? not everywhere
 *environment variables (getenv, fprintf(stderr, ...))
 */

// options
//-v --version
//-h --help
// environment variables: LAB11DEBUG - enabling or disabling debug info

void walking(int debug_state, char *dir, char *desired_string) {
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
            if (errno != 0) {
            	if(debug_state) fprintf(stderr, "Error reading file %s: %s\n", p->d_name, strerror(errno)); 
	            continue;
            }
            // man readdir - EOF returned on both EOF and errors
                break;
        }
        // strcmp returns 0 if strings are equal
        // so we check that none of them is 0
        if (strcmp(p->d_name, ".") && strcmp(p->d_name, "..")) {
            //// for(int i = 0; i < level; i++) printf("--"); //printing levels!!!levels removed from code
            // printf("%s - [%d]\n", p->d_name, p->d_type);
            if (p->d_type == DT_DIR) {
            	if(strlen(dir)+strlen(p->d_name) > PATH_MAX){
            		if(debug_state) fprintf(stderr, "Path too long to open dir %s\n", p->d_name);
            		continue;
            	}
                char newdir[PATH_MAX];
                sprintf(newdir, "%s/%s", dir, p->d_name);  //
                // new directory for opendir() and readdir()
                walking(debug_state, newdir, desired_string);
            } else {
                short found_flag = 0;
                chdir(dir);
                FILE *fp = fopen(p->d_name, "r");
                if (!fp) {
                   if(debug_state) fprintf(stderr,"error opening file %s: %s\n", p->d_name, strerror(errno));
                    
                } else {
                    char *buf = NULL;
                    size_t buf_len = 0;
                    ssize_t read;
                    while ((read = getline(&buf, &buf_len, fp)) != -1) {
                        if (strstr(buf, desired_string) != NULL) found_flag = 1;
                    }
                    free(buf);
                }
                fclose(fp);
                if (found_flag)
                    printf("found file: %s/%s \n", get_current_dir_name(),
                           p->d_name);
            }
        }
    }

    closedir(d);
    return;
}

int main(int argc, char *argv[]) {
	if(getenv("LAB11DEBUG") != NULL) printf("%s\n",getenv("LAB11DEBUG"));
    int option_index = 0;
    //getopt loop
    for (;;) {
        static struct option long_options[] = {{"version", no_argument, 0, 0},
                                               {"help", no_argument, 0, 0}};
        int c = getopt_long(argc, argv, "vh", long_options, &option_index);
        if (c == -1) break;
        switch (c) {
            case 0:
                if (strcmp(long_options[option_index].name, "version") == 0) {
                    printf(
                        "Lab1.1 version 0.7 Волков Сергей Алексеевич N32471 2 "
                        "вариант\n");
                    exit(EXIT_SUCCESS);
                } else if (strcmp(long_options[option_index].name, "help") ==
                           0) {
                    printf("Usage: %s <dir> <search_query>\n\n", argv[0]);
                    printf(
                        "options:\n -h, --help \t display argument "
                        "information\n -v, --version - display version info\n");
                    exit(EXIT_SUCCESS);
                }
                break;
            case 'v':
                printf(
                    "Lab1.1 version 0.7 Волков Сергей Алексеевич N32471 2 "
                    "вариант\n");
                exit(EXIT_SUCCESS);
                break;
            case 'h':
                printf("Usage: %s <dir> <search_query>\n", argv[0]);
                printf(
                    "options:\n -h, --help \t display argument information\n "
                    "-v, --version - display version info\n");
                exit(EXIT_SUCCESS);
                break;
            default:
                break;
        }
    }
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <dir> <search_query>\n\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    walking(0, argv[1], argv[2]);
    return 0;
}

