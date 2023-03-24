#define _GNU_SOURCE  // for get_current_dir_name
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>  //for booleans
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include "utils.h"

#define PATH_MAX 4096
// var: 19
//
//Опция: --ipv4-addr-bin <значение>
//Назначение: поиск файлов, содержащих заданное значение IPv4-адреса в бинарной
//(little-endian или big-endian) форме, т. е. в виде четырех последовательных октетов
//с заданными значениями.
//Пример: --ipv4-addr-bin 192.168.8.1

/*
options:
-h 
-v
-O - "or" option for plugins
-A - "and" option for plugins
-N - "not" option for plugins
-P <dir> - plugin directory
*/


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
                        "Lab1.2 version 0.1 Волков Сергей Алексеевич N32471 19"
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

