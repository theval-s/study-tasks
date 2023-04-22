#define _GNU_SOURCE  // for get_current_dir_name
#include "utils.h"
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>  //for booleans
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <dlfcn.h>


#define PATH_MAX 4096
// var: 19
//
//Опция: --ipv4-addr-bin <значение>
//Назначение: поиск файлов, содержащих заданное значение IPv4-адреса в бинарной
//(little-endian или big-endian) форме, т. е. в виде четырех последовательных октетов
//с заданными значениями.
//Пример: --ipv4-addr-bin 192.168.8.1
//

/*options:
-h 
-v
-O - "or" option for plugins
-A - "and" option for plugins
-N - "not" option for plugins
-P <dir> - plugin directory


notable: check examples in GDrive 
*/

/* TODO:
// *lib -- 3/5
// *call for one
// *call for several
// *-O,-A, -N options
// *more error catching
// *fix for cwd -- done
*/

int main(int argc, char *argv[]) {
    char prog_path[PATH_MAX];
    strncpy(prog_path, argv[0], strlen(argv[0])-13);
    chdir(prog_path);

    int option_index = 0;
    // getopt loop
    for (;;) {
        static struct option long_options[] = {
            {"version", 0, NULL, 0},
            {"help", 0, NULL, 0},
            {"ipv4-addr-bin", 1, NULL, 0},
            {0, 0, 0, 0}};
        int c = getopt_long(argc, argv, "vh", long_options, &option_index);
        if (c == -1) break;
        if (c != 0) {
            fprintf(stderr, "Error: failed to parse options");
            exit(EXIT_FAILURE);
        }
#ifndef ALLOW_OPT_ABBREV
        int idx = (long_options + option_index)->has_arg ? 2 : 1;
        const char *actual_opt_name = argv[optind - idx] + 2; // +2 for -- before option
  
              const char *found_opt_name = (long_options + option_index)->name;
        if (strcmp(actual_opt_name, found_opt_name)) {
            // It's probably abbreviated name, which we do not allow
            fprintf(stderr, "Error: unknown option: %s\n", argv[optind - idx]);
            exit(0);
        }
#endif
        /*
        memcpy(opts_to_pass + opts_to_pass_len, longopts + opt_ind, sizeof(struct option));
        // Argument (if any) is passed in flag
        if ((longopts + opt_ind)->has_arg) {
            // Mind this!
            // flag is of type int*, but we are passing char* here (it's ok to do so).
            (opts_to_pass + opts_to_pass_len)->flag = (int*)strdup(optarg);
        }
        opts_to_pass_len++;        
    }*/ //need to deal with this to pass args :(((
        switch (c) {
            case 'v':
                printf(
                    "Lab1.2 version 0.2 Волков Сергей Алексеевич N32471 2 "
                    "вариант\n");
                exit(EXIT_SUCCESS);
                break;
            case 'h':
                printf("Usage: %s <dir> <search_query>\n", argv[0]);
                printf(
                    "options:\n -h, --help \t display argument information\n "
                    "-v, --version - display version info\n"
                    "If LAB1DEBUG environment variable is set to 1 or true - "
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
    if (getenv("LAB1DEBUG") != NULL)
        env_debug = (strcmp(getenv("LAB1DEBUG"), "1") == 0 ||
                     (strcmp(getenv("LAB1DEBUG"), "true") == 0));
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
    //lib testing stuff
    void *lib = dlopen("libvsaN32471.so",  RTLD_LAZY | RTLD_GLOBAL);
    if (!lib)  {
        printf("ERROR: dlopen: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    printf("%s\n", get_current_dir_name());
    int (*file_check)(const char*, struct option*, size_t) = dlsym(lib, "plugin_process_file");
    struct option tes[] = {{"ipv4-addr-bin", 1, (int*)"192.168.3.1", 0}};
    printf("lib returned: %d", file_check("file.bin", tes, 1));
    //walking(env_debug, argv[1], argv[2]);
    return 0;
}

/*
for testing:
make 
sudo cp libvsaN32471.so usr/lib
./lab1vsaN32471 /home "123"
*/