#define _GNU_SOURCE  // for get_current_dir_name
#include <dirent.h>  //for opendir & readdir()
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>  //for booleans
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>  //for opendir()
#include <unistd.h>     // for chdir

// var: 2 & even
// opendir/readdir with recursion
// searching for files containing string of utf-8 bytes (input)
// example of launching: ./lab11vsaN32471 /home "лавандовый раф"

// options
//-v --version
//-h --help
// environment variables: LAB11DEBUG - enabling debug info

void walking(short debug_state, char *dir, char *desired_string) {
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
                printf("Error reading file %s: %s\n", p->d_name,
                       strerror(errno));
                continue;
            }
            // man readdir - EOF returned on both EOF and errors
            break;
        }
        // strcmp returns 0 if strings are equal
        // so we check that none of them is 0
        if (strcmp(p->d_name, ".") != 0 && strcmp(p->d_name, "..") != 0) {
            // printf("%s - [%d]\n", p->d_name, p->d_type);
            if (p->d_type == DT_DIR) {
                if (strlen(dir) + strlen(p->d_name) > PATH_MAX) {
                    printf("Path too long to open dir %s\n", p->d_name);
                    continue;
                }
                char newdir[PATH_MAX];
                sprintf(newdir, "%s/%s", dir, p->d_name);  //
                // new directory for opendir() and readdir()
                // printf("NEW DIR%s\n", newdir);
                walking(debug_state, newdir, desired_string);
            } else {
                if (debug_state)
                    fprintf(stderr, "reading file %s:\n", p->d_name);
                bool found_flag = 0;
                chdir(dir);
                FILE *fp = fopen(p->d_name, "r");
                if (!fp) {
                    printf("error opening file %s: %s\n", p->d_name,
                           strerror(errno));
                    printf("at dir:%s\n", dir);
                } else {
                    char *buf = NULL;
                    size_t buf_len = 0;
                    ssize_t read;
                    while ((read = getline(&buf, &buf_len, fp)) != -1) {
                        if (strstr(buf, desired_string) != NULL) found_flag = 1;
                    }
                    free(buf);
                    fclose(fp);
                }
                if (found_flag) printf("found file: %s/%s \n", dir, p->d_name);
            }
        }
    }
    closedir(d);
    return;
}

int main(int argc, char *argv[]) {
    int option_index = 0;
    // getopt loop
    for (;;) {
        static struct option long_options[] = {
            {"version", no_argument, NULL, 1},
            {"help", no_argument, NULL, 2},
            {0, 0, 0, 0}};
        int c = getopt_long(argc, argv, "vh", long_options, &option_index);
        if (c == -1) break;
        switch (c) {
            case 1:
                if (strcmp(argv[optind - 1], "--version") == 0) {
                    printf(
                        "Lab1.1 version 0.7 Волков Сергей Алексеевич N32471 2 "
                        "вариант\n");

                    exit(EXIT_SUCCESS);
                } else {
                    printf("Option not recognized. Use %s --help for help\n",
                           argv[0]);
                    exit(EXIT_FAILURE);
                }
            case 2:
                if (strcmp(argv[optind - 1], "--help") == 0) {
                    printf("Usage: %s <dir> <search_query>\n", argv[0]);
                    printf(
                        "options:\n -h, --help \t display argument "
                        "information\n "
                        "-v, --version - display version info\n"
                        "If LAB11DEBUG environment variable is set to 1 or "
                        "true - "
                        "DEBUG MODE\n");
                    exit(EXIT_SUCCESS);
                } else {
                    printf("Option not recognized. Use %s --help for help\n",
                           argv[0]);
                    exit(EXIT_FAILURE);
                }
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
                    "-v, --version - display version info\n"
                    "If LAB11DEBUG environment variable is set to 1 or true - "
                    "DEBUG MODE\n");
                exit(EXIT_SUCCESS);
                break;
            default:
                break;
        }
    }
    if (argc != 3) {
        // if there are 2 options it still launches the program
        fprintf(stderr, "Not enough arguments. Use %s --help for help\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
    bool env_debug = false;
    if (getenv("LAB11DEBUG") != NULL)
        env_debug = (strcmp(getenv("LAB11DEBUG"), "1") == 0 ||
                     (strcmp(getenv("LAB11DEBUG"), "true") == 0));
    // printf("%d\n", env_debug); - to check if debug mode is initiated

    // to get absolute path for folder
    if (argv[1][0] != '/') {
        char *cwd = get_current_dir_name();
        if (cwd) {
            char abs_path[PATH_MAX];
            sprintf(abs_path, "%s/%s", cwd, argv[1]);
            argv[1] = abs_path;
            free(cwd);
        }
    }

    walking(env_debug, argv[1], argv[2]);
    return 0;
}

