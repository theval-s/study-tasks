#ifndef __UTILS__
#define __UTILS__
#include <getopt.h>
#include <stddef.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "plugin_api.h"
typedef int (*process_file_t)(const char *,
                        struct option *,
                        size_t);
void open_libs(struct plugin_option *in_opts[], size_t *in_opts_c, char *dir, process_file_t **proc_file, size_t *proc_file_c, void ***dlibs, size_t *libcnt);
void walking(struct option in_opts[], size_t in_opts_len, char *dir,process_file_t *proc_file, size_t proc_file_c, const int narg, const int oaarg);

#endif
