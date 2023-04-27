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
    char prog_path[PATH_MAX];
    short cnt = 0;
    for (size_t i = strlen(argv[0]) - 1; i > 0; i--)
    {
        if (argv[0][i] != '/')
            cnt++;
        else
            break;
    }
    memcpy(prog_path, argv[0], strlen(argv[0]) - cnt);
    int t = chdir(prog_path);
    if (t != 0)
        fprintf(stderr, "couldn't change working dir to program location: %s", strerror(errno));
    char *cwdbuf = NULL; 
    cwdbuf = getcwd(cwdbuf, PATH_MAX);
    strcpy(prog_path, cwdbuf);
    free(cwdbuf);
    char libpath[PATH_MAX];
    sprintf(libpath, "%s/", prog_path);

    size_t in_opts_c = 0;
    struct plugin_option *in_opts = NULL; // allocated in open_libs func - need to free it later
    process_file_t *proc_file_arr = NULL; // allocated in open_libs! free it later
    size_t proc_file_c = 0;
    void **dls = NULL;
    size_t libcnt = 0;
    short narg = 0, oaarg = 0;
    for (int i = 1; i < argc; i++)
    {
        if (strlen(argv[i]) == 2 && argv[i][0] == '-' && argv[i][1] != '-')
        {
            switch (argv[i][1])
            {
            case 'P':
                sprintf(libpath, "%s/", argv[i + 1]);
                if (getenv("LAB1DEBUG") != NULL)
                    printf("DBG: libpath changed to:%s\n\n", libpath);
                break;
            case 'v':
                printf(
                    "Lab1.2 version 0.7 Волков Сергей Алексеевич N32471 "
                    "19 вариант\n");
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
                open_libs(&in_opts, &in_opts_c, libpath, &proc_file_arr, &proc_file_c, &dls, &libcnt);
                printf("available plugin options: \n");
                for (size_t i = 0; i < in_opts_c; i++)
                {
                    printf("\t --%s \t%s\n", in_opts[i].opt.name, in_opts[i].opt_descr);
                }
                if (dls)
                {
                    for (size_t i = 0; i < libcnt; i++)
                    {
                        dlclose(dls[i]);
                    }
                    free(dls);
                }
                if (in_opts)
                    free(in_opts);
                if (proc_file_arr)
                    free(proc_file_arr);
                exit(EXIT_SUCCESS);
                break;
            case 'O':
                oaarg = 1;
                break;
            case 'N':
                narg = 1;
                break;
            default:
                printf("Error: failed to parse short options!\n");
                exit(EXIT_FAILURE);
                break;
            }
        }
    }

    open_libs(&in_opts, &in_opts_c, libpath, &proc_file_arr, &proc_file_c, &dls, &libcnt);
    if (in_opts_c == 0 || proc_file_c == 0)
    {
        printf("Failed to find plugin options! Check the directory!\n");
        if (dls)
        {
            for (size_t i = 0; i < libcnt; i++)
            {
                dlclose(dls[i]);
            }
            free(dls);
        }
        if (in_opts)
            free(in_opts);
        if (proc_file_arr)
            free(proc_file_arr);
        exit(EXIT_FAILURE);
    }
    struct option long_options[in_opts_c + 1];
    for (size_t i = 0; i < in_opts_c; i++)
    {
        long_options[i] = in_opts[i].opt;
    }
    long_options[in_opts_c] = (struct option){0, 0, 0, 0};
    struct option *walk_opts = NULL;
    size_t walk_opts_len = 0;
    int option_index = 0;
    // getoptlong loop
    for (;;)
    {
        int c = getopt_long(argc, argv, "hvP:OAN", long_options, &option_index);
        if (c == -1)
            break;

        if (c != 0)
        {
            continue;
        }
        walk_opts_len++;
        walk_opts = realloc(walk_opts, walk_opts_len * sizeof(struct option));
        walk_opts[walk_opts_len - 1] = long_options[option_index];
        if ((long_options + option_index)->has_arg)
        {
            walk_opts[walk_opts_len - 1].flag = (int *)(optarg);
        }
    }
    if (walk_opts_len == 0)
    {
        printf("No correct search options!\nUsage:%s [options] <dir>\n-h for help and available options\n", argv[0]);
        if (dls)
        {
            for (size_t i = 0; i < libcnt; i++)
            {
                dlclose(dls[i]);
            }
            free(dls);
        }
        if (in_opts)
            free(in_opts);
        if (walk_opts)
            free(walk_opts);
        if (proc_file_arr)
            free(proc_file_arr);
        exit(EXIT_FAILURE);
    }
    // to get absolute path for folder
    if (argv[argc - 1][0] != '/')
    {
        char *cwd = get_current_dir_name();
        if (cwd)
        {
            char abs_path[PATH_MAX];
            sprintf(abs_path, "%s/%s", cwd, argv[argc - 1]);
            argv[argc - 1] = abs_path;
            free(cwd);
        }
    }
    walking(walk_opts, walk_opts_len, argv[argc - 1], proc_file_arr, proc_file_c, narg, oaarg);

    if (dls)
    {
        for (size_t i = 0; i < libcnt; i++)
        {
            dlclose(dls[i]);
        }
        free(dls);
    }
    if (in_opts)
        free(in_opts);
    if (walk_opts)
        free(walk_opts);
    if (proc_file_arr)
        free(proc_file_arr);
    return 0;
}
