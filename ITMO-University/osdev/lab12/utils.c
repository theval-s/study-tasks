#include "utils.h"
#include "plugin_api.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> //for opendir()
#include <unistd.h>
#include <dlfcn.h>
#define PATH_MAX 4096
/// opens all libraries using refs to objects to save them without making them global variables 
//void open_libs
void open_libs(char *dir, size_t *plug_cnt, plug_t *plug_arr[])
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
                void *pf = dlsym(dl, "plugin_process_file");
                if (pf == NULL)
                {
                    fprintf(stderr, "Error: dlsym() failed for '%s' lib:%s\n", p->d_name, dlerror());
                    dlclose(dl);
                    continue;
                }
                
                (*plug_arr) = realloc((*plug_arr), sizeof(plug_t) * ((*plug_cnt)+1));
                if((*plug_arr) == NULL){
                    fprintf(stderr, "CRITICAL: realloc failed in open_libs(%s)", dir);
                    dlclose(dl);
                    exit(EXIT_FAILURE);
                }
                (*plug_arr)[(*plug_cnt)].dl_handle = dl;
                (*plug_arr)[(*plug_cnt)].proc_file = (process_file_t)pf;
                (*plug_arr)[(*plug_cnt)].pi =plug_info;
                (*plug_cnt)++;
            }
            else
                fprintf(stderr, "Err: dlopen() failed for lib '%s'. Reason:%s\n", p->d_name, dlerror());
        }
    }
    closedir(d);
}

//checks all files using plugin_process_file functions array
void walking(char *dir, struct option **in_opts, size_t *in_opts_len, plug_t plug_arr[], size_t libcnt, const int narg, const int oaarg)
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
                // printf("NEW DIR%s\n", newdir); //debug
                walking(newdir, in_opts, in_opts_len, plug_arr, libcnt, narg, oaarg);
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
                for (size_t i = 0; i < libcnt; i++)
                {
                    if (in_opts[i] == NULL)
                        continue;
                    found_prev = found;
                    found = plug_arr[i].proc_file(p->d_name, in_opts[i], in_opts_len[i]);
                    if (found < 0)
                    {
                        fprintf(stderr, "Error in lib №%ld! :%s\n", i+1, strerror(errno));
                        if(errno == EINVAL || errno == ERANGE){
                            //skip library if there is error with arguments (elss error msg)
                            fprintf(stderr, "Skipping it because of invalid arguments...\n");
                            free(in_opts[i]);
                            in_opts[i] = NULL;
                        }
                        errno = 0;
                        found = found_prev;
                    } else  found = (found == 0) ? 1 : 0;
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
