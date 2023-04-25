#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "plugin_api.h"
// var: 19
//
// Опция: --ipv4-addr-bin <значение>
// Назначение: поиск файлов, содержащих заданное значение IPv4-адреса в бинарной
//(little-endian или big-endian) форме, т. е. в виде четырех последовательных октетов
// с заданными значениями.
// Пример: --ipv4-addr-bin 192.168.8.1
//
static char *lib_name = "libvsaN32471.so";

int plugin_get_info(struct plugin_info *ppi)
{
    ppi->plugin_purpose = "search for binary ipv4";
    ppi->plugin_author = "Волков Сергей Алексеевич, N32471";
    static struct plugin_option opts[] = {
        {{"ipv4-addr-bin", required_argument, 0, 0}, "Target IPv4 addr in xxx.xxx.xxx.xxx"}};
    ppi->sup_opts = opts;
    ppi->sup_opts_len = sizeof(opts) / sizeof(opts[0]);
    return 0;
}
/* TODO
-basic outline -- done
-parsing char ip value to remove dots :( -- done
-file-check logic -- done
-error catching -- done
-debug mode
*/
static int isEqual(unsigned char *buf, unsigned char *s)
{
    int ret = 1;
    for (size_t i = 0; i < 4; i++)
    {
        if (buf[i] != s[i])
            return 0;
    }
    return ret;
}

int plugin_process_file(const char *fname,
                        struct option in_opts[],
                        size_t in_opts_len)
{
    if (!fname || !in_opts || !in_opts_len)
    {
        fprintf(stderr, "Argument missing, lib_name:%s\n\n", lib_name);
    }

    char ip[15];
    for (size_t i = 0; i < in_opts_len; i++)
    {
        if (!strcmp(in_opts[i].name, "ipv4-addr-bin"))
        {
            if (strlen((char *)in_opts[i].name) >= 15)
            {
                errno = EINVAL;
                return -1;
            }
            strcpy(ip, (char *)in_opts[i].flag);
        }
    }
    char *endptr;
    unsigned char nums[4];
    unsigned int t = strtol(strtok_r(ip, ".", &endptr), NULL, 10);
    if (t > 255)
    {
        errno = EINVAL;
        return -1;
    }
    nums[0] = t;
    for (int i = 1; i < 4; i++)
    {
        t = strtol(strtok_r(NULL, ".", &endptr), NULL, 10);
        if (t > 255)
        {
            errno = EINVAL;
            return -1;
        }
        nums[i] = t;
    }
    unsigned char be[4], le[4];
    for (int i = 0; i < 4; i++)
    {
        be[i] = (unsigned char)nums[i];
        le[3 - i] = (unsigned char)nums[i];
    }
    /*for(int i = 0; i < 4; i++){
        printf("nums[X]:%X\tbe[X]:%X\tle[X]:%X\n",nums[i], (unsigned char)be[i], (unsigned char)le[i]);
    }*/
    int found_flag = 0;
    FILE *fp = fopen(fname, "rb");
    if (!fp)
    {
        return -1;
        // errno set by fopen
    }
    unsigned char buf[4];
    fread(buf, sizeof(char), 4, fp);
    // printf("first 4 bytes of file: %X%X%X%X\n", (unsigned char)buf[0],(unsigned char)buf[1],(unsigned char)buf[2],(unsigned char)buf[3]);

    if (isEqual(buf, be) || isEqual(buf, le))
    {
        if (getenv("LAB1DEBUG") != NULL)
            fprintf(stderr, "in file: %s found: %X%X%X%X\n", fname, buf[0], buf[1], buf[2], buf[3]);
        return 1;
    }
    while (!feof(fp))
    {
        char t[1];
        fread(t, sizeof(char), 1, fp);
        for (short i = 0; i < 3; i++)
            buf[i] = buf[i + 1];
        buf[3] = t[0];
        if (isEqual(buf, be) || isEqual(buf, le))
            return 1;
    }
    return found_flag;
}
// jealousy turning saints into the sea