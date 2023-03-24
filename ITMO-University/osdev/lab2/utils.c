#include "utils.h"
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>  //for opendir()
#include <unistd.h>     // for chdir

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
