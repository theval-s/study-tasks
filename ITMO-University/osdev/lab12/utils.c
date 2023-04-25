#include "utils.h"
#include "plugin_api.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h> //for opendir()
#include <unistd.h>    // for chdir
#include <dlfcn.h>
#define PATH_MAX 4096

void open_libs(struct plugin_option *in_opts[], size_t *in_opts_c, char *dir)
{
    DIR *d = opendir(dir);
    if (d == NULL)
    {
        printf("Err: while searching for libraries failed to opendir() %s\n", dir);
        // exit(EXIT_FAILURE);
        return;
    }
    for (;;)
    {
        errno = 0;
        struct dirent *p = readdir(d);
        if (p == NULL)
        {
            if (errno != 0)
            {
                printf("Error reading file %s: %s\n", p->d_name,
                       strerror(errno));
                continue;
            }
            break;
        }
        if (strcmp(p->d_name, ".") != 0 && strcmp(p->d_name, "..") != 0 && p->d_type != DT_DIR && strstr(p->d_name, ".so") != NULL)
        {
            if (getenv("LAB1DEBUG") != NULL)
                printf("DBG: found library!:%s\n", p->d_name);
            char pth[PATH_MAX];
            sprintf(pth, "%s/%s", dir, p->d_name);
            void *dl = dlopen(pth, RTLD_LAZY);
            if (dl != NULL)
            {
                struct plugin_info plug_info = {0};
                typedef int (*getinfo_fun)(struct plugin_info *);
                void *dlfun = dlsym(dl, "plugin_get_info");
                if (dlfun == NULL)
                {
                    fprintf(stderr, "Err: dlsym() failed:%s\n", dlerror());
                    continue;
                }
                getinfo_fun func = (getinfo_fun)dlfun;

                int info = func(&plug_info);
                if (info < 0)
                {
                    fprintf(stderr, "get_plugin_info failed for lib '%s'\n", p->d_name);
                }
                if (plug_info.sup_opts_len == 0)
                {
                    fprintf(stderr, "Plugin '%s' issue: no options?", p->d_name);
                    continue;
                }
                (*in_opts_c) += plug_info.sup_opts_len;
                (*in_opts) = realloc((*in_opts), (*in_opts_c) * sizeof(struct plugin_option));
                if ((*in_opts) == NULL)
                {
                    printf("Error! realloc() failed");
                    exit(EXIT_FAILURE);
                }
                for (size_t i = 0; i < plug_info.sup_opts_len; i++)
                {
                    (*in_opts)[(*in_opts_c) - plug_info.sup_opts_len + i] = plug_info.sup_opts[i];
                    //printf("added option to opts!%s \t%s\n", plug_info.sup_opts[i].opt.name, plug_info.sup_opts[i].opt_descr);
                }
            }
            else
                fprintf(stderr, "Err: dlopen() failed for lib '%s'. Reason:%s\n", p->d_name, dlerror());
        }
    }
    closedir(d);
}

void walking(struct option in_opts[], size_t in_opts_len, char *dir, char *desired_string, const int narg, const int oaarg)
{
    DIR *d = opendir(dir);
    if (d == NULL)
    {
        printf("Failed to opendir() %s\n", dir);
        return;
    }
    // just to compile it for test
    if (narg)
        printf("no");
    if (oaarg)
        printf("or");
    in_opts_len++;
    in_opts[0].flag = (int *)"test";
    for (;;)
    {
        errno = 0;
        struct dirent *p = readdir(d);
        /*
    dirent{ d_ino - inode number, d_off - telldir, d_reclen - length,
    d_type - type of file, d_name[256] - filename};
    d_type - DT_DIR - directory, DT_LNK - symbolic link, DT_REG - file, etc
        */
        if (p == NULL)
        {
            if (errno != 0)
            {
                printf("Error reading file %s: %s\n", p->d_name,
                       strerror(errno));
                continue;
            }
            // man readdir - EOF returned on both EOF and errors
            break;
        }
        // strcmp returns 0 if strings are equal
        // so we check that none of them is 0
        if (strcmp(p->d_name, ".") != 0 && strcmp(p->d_name, "..") != 0)
        {
            // printf("%s - [%d]\n", p->d_name, p->d_type);
            if (p->d_type == DT_DIR)
            {
                if (strlen(dir) + strlen(p->d_name) > PATH_MAX)
                {
                    printf("Path too long to open dir %s\n", p->d_name);
                    continue;
                }
                char newdir[PATH_MAX];
                sprintf(newdir, "%s/%s", dir, p->d_name); //
                // new directory for opendir() and readdir()
                // printf("NEW DIR%s\n", newdir);
                walking(in_opts, in_opts_len, newdir, desired_string, narg, oaarg);
            }
            else
            {
            }
            /* need to change this to use library functions instead:
            {
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
                if (found_flag) {
                    printf("found file: %s/%s \n", dir, p->d_name);
                }
            }
            */
        }
    }
    closedir(d);
    return;
}
