#include "utils.h"
#include "plugin_api.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> //for opendir()
#include <unistd.h>
#include <dlfcn.h>
#define PATH_MAX 4096

void open_libs(struct plugin_option *in_opts[], size_t *in_opts_c, char *dir, process_file_t **proc_file, size_t *proc_file_c, void ***dlibs, size_t *libcnt, struct plugin_info **pi)
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
                    dlclose(dl);
                    continue;
                }
                getinfo_fun func = (getinfo_fun)dlfun;

                int info = func(&plug_info);
                if (info < 0)
                {
                    fprintf(stderr, "get_plugin_info failed for lib '%s', trying next...\n", p->d_name);
                    dlclose(dl);
                    continue;
                }
                if (plug_info.sup_opts_len == 0)
                {
                    fprintf(stderr, "Plugin '%s' issue: no options! Trying next plugin...\n", p->d_name);
                    dlclose(dl);
                    continue;
                }
                (*pi) = realloc((*pi), sizeof(struct plugin_info) * ((*libcnt) + 1));
                (*pi)[(*libcnt)] = plug_info;
                (*in_opts_c) += plug_info.sup_opts_len;
                (*in_opts) = realloc((*in_opts), (*in_opts_c) * sizeof(struct plugin_option));
                if ((*in_opts) == NULL)
                {
                    fprintf(stderr, "Error! realloc() failed\n");
                    dlclose(dl);
                    if ((*dlibs))
                    {
                        for (size_t i = 0; i < (*libcnt); i++)
                        {
                            dlclose((*dlibs)[i]);
                        }
                        free((*dlibs));
                    }
                    free((*in_opts));
                    free((*proc_file));
                    exit(EXIT_FAILURE);
                }
                for (size_t i = 0; i < plug_info.sup_opts_len; i++)
                {
                    (*in_opts)[(*in_opts_c) - plug_info.sup_opts_len + i] = plug_info.sup_opts[i];
                    // printf("added option to opts!%s \t%s\n", plug_info.sup_opts[i].opt.name, plug_info.sup_opts[i].opt_descr);
                }
                void *pf = dlsym(dl, "plugin_process_file");
                if (pf == NULL)
                {
                    fprintf(stderr, "Error: dlsym() failed for '%s' lib:%s\n", p->d_name, dlerror());
                    dlclose(dl);
                    continue;
                }
                (*proc_file_c)++;
                (*proc_file) = realloc((*proc_file), (*proc_file_c) * sizeof(process_file_t));
                (*proc_file)[(*proc_file_c) - 1] = (process_file_t)pf;
                (*libcnt)++;
                (*dlibs) = realloc((*dlibs), sizeof(dl) * (*libcnt));
                (*dlibs)[(*libcnt) - 1] = dl;
            }
            else
                fprintf(stderr, "Err: dlopen() failed for lib '%s'. Reason:%s\n", p->d_name, dlerror());
        }
    }
    closedir(d);
}

void walking(struct option **in_opts, size_t *in_opts_len, char *dir, process_file_t *proc_file, size_t proc_file_c, const int narg, const int oaarg)
{
    DIR *d = opendir(dir);
    if (d == NULL)
    {
        printf("Failed to opendir() %s\n", dir);
        return;
    }

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
                walking(in_opts, in_opts_len, newdir, proc_file, proc_file_c, narg, oaarg);
            }
            else
            {
                int ch = chdir(dir);
                if (ch != 0)
                {
                    fprintf(stderr, "couldn't change working dir: %s\n", strerror(errno));
                    continue;
                }
                int found = 0;
                int found_prev = 0;
                for (size_t i = 0; i < proc_file_c; i++)
                {
                    if (in_opts[i] == NULL)
                        continue;
                    found_prev = found;
                    found = (proc_file[i](p->d_name, in_opts[i], in_opts_len[i]) == 0 ? 1 : 0);
                    if (found == -1)
                    {
                        fprintf(stderr, "Error in lib №%ld! :%s\n", i, strerror(errno));
                        found = found_prev;
                    }
                    if (oaarg == 0 && found == 0)
                        break;
                    else if (oaarg == 1 && found == 1)
                        break;
                }
                if (narg != 1 && found == 1)
                    printf("found file: %s/%s \n", dir, p->d_name);
                else if (narg == 1 && found == 0)
                    printf("found file: %s/%s \n", dir, p->d_name);
            }
        }
    }
    closedir(d);
    return;
}
