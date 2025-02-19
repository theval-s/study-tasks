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
*/

void cleanup(size_t libcnt, plug_t plug_arr[]);
int main(int argc, char *argv[])
{

    // to change cwd to program folder (to search for libs)
    char prog_path[PATH_MAX] = "";
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

    size_t libcnt = 0;
    plug_t *plug_arr = NULL;
    short narg = 0, oaarg = 0;
    for (int i = 1; i < argc; i++)
    { // manually checking for -P to change dir before getopt_long
        if (strlen(argv[i]) == 2 && argv[i][0] == '-' && argv[i][1] != '-')
        {
            if (argv[i][1] == 'P')
            {
                if (strlen(argv[i + 1]) > PATH_MAX)
                {
                    fprintf(stderr, "Argument after '-P' too long!");
                    cleanup(libcnt, plug_arr);
                    exit(EXIT_FAILURE);
                }
                else
                    strcpy(libpath, argv[i + 1]);
            }
        }
    }
    open_libs(libpath, &libcnt, &plug_arr);

    size_t in_opts_c = 0;
    for (size_t i = 0; i < libcnt; i++)
        in_opts_c += plug_arr[i].pi.sup_opts_len;
    struct option long_options[in_opts_c + 1];
    in_opts_c = 0;
    for (size_t i = 0; i < libcnt; i++)
    {
        for (size_t j = 0; j < plug_arr[i].pi.sup_opts_len; j++)
        {
            long_options[in_opts_c++] = plug_arr[i].pi.sup_opts[j].opt;
        }
    }
    // making the option array
    long_options[in_opts_c] = (struct option){0, 0, 0, 0};
    struct option **walk_opts = calloc(libcnt, sizeof(struct option *));
    if(!walk_opts){
        fprintf(stderr, "CRITICAL: failed to calloc() walk_opts\n");
        exit(EXIT_FAILURE);
    }
    size_t *walk_opts_len = calloc(libcnt, sizeof(size_t));
    if(!walk_opts_len){
        fprintf(stderr, "CRITICAL: failed to calloc() walk_opts_len\n");
        exit(EXIT_FAILURE);
    }
    if (walk_opts == NULL || walk_opts_len == NULL)
    {
        printf("Malloc failed! Exiting...");
        if (walk_opts)
        {
            for (size_t z = 0; z < libcnt; z++)
            {
                if (walk_opts[z])
                    free(walk_opts[z]);
            }
            free(walk_opts);
        }
        if (walk_opts_len)
            free(walk_opts_len);
        cleanup(libcnt, plug_arr);
        exit(EXIT_FAILURE);
    }
    int option_index = 0;
    // getoptlong loop
    for (;;)
    {
        int c = getopt_long(argc, argv, "hvP:OAN", long_options, &option_index);
        if (c == -1)
            break;

        if (c != 0)
        {
            switch (c)
            {
            case 'P':
                // уже разобрано
                break;
            case 'v':
                printf(
                    "Lab1.2 version 1.01 Волков Сергей Алексеевич N32471 "
                    "19 вариант\n");
                if (walk_opts)
                {
                    for (size_t z = 0; z < libcnt; z++)
                    {
                        if (walk_opts[z])
                            free(walk_opts[z]);
                    }
                    free(walk_opts);
                }
                free(walk_opts_len);
                cleanup(libcnt, plug_arr);
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
                for (size_t i = 0; i < libcnt; i++)
                {
                    printf("plugin №%ld:\n", i + 1);
                    for (size_t j = 0; j < plug_arr[i].pi.sup_opts_len; j++)
                    {
                        printf("--%s\t\t%s\n", plug_arr[i].pi.sup_opts[j].opt.name, plug_arr[i].pi.sup_opts[j].opt_descr);
                    }
                    printf("\n");
                }
                if (walk_opts)
                {
                    for (size_t z = 0; z < libcnt; z++)
                    {
                        if (walk_opts[z])
                            free(walk_opts[z]);
                    }
                    free(walk_opts);
                }
                free(walk_opts_len);
                cleanup(libcnt, plug_arr);
                exit(EXIT_SUCCESS);
                break;
            case 'O':
                oaarg = 1;
                break;
            case 'N':
                narg = 1;
                break;
            case 'A':
                oaarg = 0;
                break;
            default:
                printf("Error: failed to parse options! Use -h to see available opts\n");
                if (walk_opts)
                {
                    for (size_t z = 0; z < libcnt; z++)
                    {
                        if (walk_opts[z])
                            free(walk_opts[z]);
                    }
                    free(walk_opts);
                }
                free(walk_opts_len);
                cleanup(libcnt, plug_arr);
                exit(EXIT_FAILURE);
                break;
            }
        }
        else
        {
            int found_lib = 0; // нашли опцию или нет, а также проверка на совп. имена
            for (size_t i = 0; i < libcnt; i++)
            {
                for (size_t j = 0; j < plug_arr[i].pi.sup_opts_len; j++)
                {
                    if (strcmp(long_options[option_index].name, plug_arr[i].pi.sup_opts[j].opt.name) == 0)
                    {
                        found_lib++;
                        break;
                    }
                }
                if (found_lib == 1)
                {
                    walk_opts_len[i]++;
                    walk_opts[i] = realloc(walk_opts[i], walk_opts_len[i] * sizeof(struct option));
                    if (walk_opts[i] == NULL)
                    {
                        fprintf(stderr, "realloc() failed in getopt_long");
                        if (walk_opts)
                        {
                            for (size_t z = 0; z < libcnt; z++)
                            {
                                if (walk_opts[z])
                                    free(walk_opts[z]);
                            }
                            free(walk_opts);
                        }
                        free(walk_opts_len);
                        cleanup(libcnt, plug_arr);
                        exit(EXIT_FAILURE);
                    }

                    // сохранение опций в отдельный массив
                    walk_opts[i][walk_opts_len[i] - 1] = long_options[option_index];
                    if ((long_options + option_index)->has_arg)
                    {
                        walk_opts[i][walk_opts_len[i] - 1].flag = (int *)(optarg);
                    }
                    break;
                }
                else if (found_lib > 1)
                { // если нашли одинаковые опции
                    fprintf(stderr, "Error: matching option names in libs! Opt name: %s\n", long_options[option_index].name);
                    cleanup(libcnt, plug_arr);
                    if (walk_opts)
                    {
                        for (size_t z = 0; z < libcnt; z++)
                        {
                            if (walk_opts[z])
                                free(walk_opts[z]);
                        }
                        free(walk_opts);
                    }
                    if (walk_opts_len)
                        free(walk_opts_len);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    int foundc = 0;
    for (size_t i = 0; i < libcnt; i++)
    {
        if (walk_opts[i] == NULL)
            printf("No options found for lib№%ld (%s), skipping it...\n", i + 1, plug_arr[i].pi.plugin_purpose);
        else
            foundc += walk_opts_len[i];
    }

    if (foundc == 0)
    {
        printf("No correct search options!\nUsage:%s [options] <dir>\n-h for help and available options\n", argv[0]);
        if (walk_opts)
        {
            for (size_t i = 0; i < libcnt; i++)
            {
                if (walk_opts_len[i] > 0)
                    free(walk_opts[i]);
            }
            free(walk_opts);
        }
        if (walk_opts_len)
            free(walk_opts_len);
        cleanup(libcnt, plug_arr);
        exit(EXIT_FAILURE);
    }
    // to get absolute path for folder
    if (argv[argc - 1][0] != '/')
    {
        char *cwd = get_current_dir_name();
        if (cwd)
        {
            char abs_path[PATH_MAX];
            if (argv[argc - 1][0] == '.' && argv[argc - 1][1] == '/')
                sprintf(abs_path, "%s%s", cwd, &(argv[argc - 1][2]));
            else
                sprintf(abs_path, "%s/%s", cwd, argv[argc - 1]);
            argv[argc - 1] = abs_path;
            free(cwd);
        }
    }
    walking(argv[argc - 1], walk_opts, walk_opts_len, plug_arr, libcnt, narg, oaarg);
    cleanup(libcnt, plug_arr);
    if (walk_opts)
    {
        for (size_t i = 0; i < libcnt; i++)
        {
            if (walk_opts_len[i] > 0)
                free(walk_opts[i]);
        }
        free(walk_opts);
        free(walk_opts_len);
    }
    return 0;
}
/// freeing args
void cleanup(size_t libcnt, plug_t plug_arr[])
{
    for (size_t i = 0; i < libcnt; i++)
    {
        if (dlclose(plug_arr[i].dl_handle) != 0)
            fprintf(stderr, "error while dlclosing(): %s\n", dlerror());
    }
    if (plug_arr)
        free(plug_arr);
}