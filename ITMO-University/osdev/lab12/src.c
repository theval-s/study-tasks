#define _GNU_SOURCE // for get_current_dir_name
#include "utils.h"
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

#define PATH_MAX 4096

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
// *lib -- 4/5
// *search for libs --done
// *plugin_options collecting --done
// *getopt_long
// *more error catching
// *fix for cwd -- done
// *add atexit to free()
*/

int main(int argc, char *argv[])
{
    // to change cwd to program folder (to search for libs)
    char prog_path[PATH_MAX - 16];
    short cnt = 0;
    for (size_t i = strlen(argv[0]) - 1; i > 0; i--)
    {
        if (argv[0][i] != '/')
            cnt++;
        else
            break;
    }
    strncpy(prog_path, argv[0], strlen(argv[0]) - cnt);
    chdir(prog_path);
    getcwd(prog_path, PATH_MAX);
    char libpath[PATH_MAX];
    sprintf(libpath, "%s/", prog_path);
    // printf("libpath:%s\n", libpath);

    size_t in_opts_c = 0;
    struct plugin_option *in_opts = NULL; // allocated in open_libs func - need to free it
    open_libs(&in_opts, &in_opts_c, libpath);
    short narg = 0, oaarg = 0;
    int opt;
    while ((opt = getopt(argc, argv, "vhP:OAN")) != -1)
    {
        switch (opt)
        {
        case 'P':
            in_opts_c = 0;
            free(in_opts);
            in_opts = NULL;
            sprintf(libpath, "%s/", optarg);
            if (getenv("LAB1DEBUG"))
                printf("DBG: libpath changed to:%s\n", libpath);
            open_libs(&in_opts, &in_opts_c, libpath);
            break;
        case 'v':
            printf(
                "Lab1.2 version 0.4 Волков Сергей Алексеевич N32471"
                "вариант\n");
            exit(EXIT_SUCCESS);
            break;
        case 'h':
            printf("Usage: %s [options] <dir>\n", argv[0]);
            printf(
                "options:\n-h \t display argument information\n"
                "-v \t- display version info\n"
                "-P <dir>\t- change library search path\n"
                "-O \t - combine plugin opts using 'or'\n"
                "-A \t - combine plugin opts using 'and' (default)\n"
                "-N \t - invert plugin opts (after combining with -A or -O)\n"
                "If LAB1DEBUG environment variable is set - "
                "debug mode\n");

            printf("available plugin options: \n");
            for (size_t i = 0; i < in_opts_c; i++)
            {
                printf("\t --%s \t%s\n", in_opts[i].opt.name, in_opts[i].opt_descr);
            }
            exit(EXIT_SUCCESS);
            break;
        case 'O':
            oaarg = 1;
            break;
        case 'N':
            narg = 1;
            break;
        default:
            printf("Error: failed to parse short options!");
            exit(EXIT_FAILURE);
            break;
        }
    }
    if (narg)
        printf("no");
    if (oaarg)
        printf("or");

    // int option_index = 0;
    //  getoptlong loop
    /*
        int c = getopt_long_only(argc, argv, NULL, long_options, &option_index);
        if (c == -1) break;
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
       else if (c != 0) {
            fprintf(stderr, "Error: failed to parse options");
            exit(EXIT_FAILURE);
        }

        memcpy(opts_to_pass + opts_to_pass_len, longopts + opt_ind, sizeof(struct option));
        // Argument (if any) is passed in flag
        if ((longopts + opt_ind)->has_arg) {
            // Mind this!
            // flag is of type int*, but we are passing char* here (it's ok to do so).
            (opts_to_pass + opts_to_pass_len)->flag = (int*)strdup(optarg);
        }
        opts_to_pass_len++;
    }*/
    // need to deal with this to pass args :(((
    // if (argc != 3) {
    //     fprintf(stderr, "Not enough arguments. Use %s --help for help\n",
    //             argv[0]);
    //      exit(EXIT_FAILURE);
    //    }

    /*
    if (argv[1][0] != '/') {
        char *cwd = get_current_dir_name();
        if (cwd) {
            char abs_path[PATH_MAX];
            sprintf(abs_path, "%s/%s", cwd, argv[1]);
            argv[1] = abs_path;
            free(cwd);
        }
    }*/
    // int (*file_check)(const char*, struct option*, size_t) = dlsym(lib, "plugin_process_file");
    // struct option tes[] = {{"ipv4-addr-bin", 1, (int*)"192.168.3.1", 0}};
    // printf("lib returned: %d", file_check("file.bin", tes, 1));
    // walking(in_opts, opts_len, argv[1], argv[argc-1]);
    return 0;
}
