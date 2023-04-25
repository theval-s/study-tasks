#ifndef __UTILS__
#define __UTILS__
#include <getopt.h>
#include <stddef.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "plugin_api.h"
void open_libs(struct plugin_option *in_opts[], size_t *in_opts_c, char *dir);
void walking(struct option in_opts[],size_t in_opts_len, char *dir, char *desired_string, const int narg, const int oaarg);

#endif
