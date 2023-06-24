#ifndef __UTILS__
#define __UTILS__
#include <getopt.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "plugin_api.h"

typedef int (*process_file_t)(const char *,
                        struct option *,
                        size_t);
typedef struct{
    void* dl_handle;
    struct plugin_info pi;
    process_file_t proc_file;
} plug_t;
void open_libs(char *dir, size_t *plug_cnt, plug_t *plug_arr[]);
void walking(char *dir, struct option **in_opts, size_t *in_opts_len, plug_t plug_arr[], size_t libcnt, const int narg, const int oaarg);

#endif
