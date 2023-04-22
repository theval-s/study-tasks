#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "plugin_api.h"

static char* lib_name = "ipv4-addr-bin.so";

int plugin_get_info(struct plugin_info * ppi){
    ppi->plugin_purpose = "search for ipv4";
    ppi->plugin_author = "Волков Сергей Алексеевич, N32471";
    ppi->sup_opts_len = 1;
    struct plugin_option opts[] = {{{"ipv4-addr-bin",1, 0, 0},"Target IPv4 addr in xxx.xxx.xxx.xx"}};
    ppi->sup_opts = opts;
    return 0;
    //possible memory leak??? idk if thats the correct way to pass opts
}
/* TODO
-basic outline -- done
-parsing char ip value to remove dots :( -- done
-file-check logic -- done
-error catching
-debug mode
*/
int plugin_process_file(const char *fname,
        struct option in_opts[],
        size_t in_opts_len){
    //char* DEBUG = getenv("LAB1DEBUG");
    if(!fname || !in_opts || !in_opts_len){
        fprintf(stderr, "Argument missing, lib_name:%s\n\n", lib_name);
    }

    char ip[15];
    for(size_t i = 0; i < in_opts_len; i++){
        if(!strcmp(in_opts[i].name, "ipv4-addr-bin")){
            strcpy(ip, (char*)in_opts[i].flag);
        }
    }
    
    char* endptr;
    unsigned char nums[4];
    nums[0] = strtol(strtok_r(ip, ".", &endptr),NULL, 10);
    //это можно переделать на for до 3х
    nums[1] = strtol(strtok_r(NULL, ".", &endptr),NULL, 10);
    nums[2] = strtol(strtok_r(NULL, ".", &endptr),NULL, 10);
    nums[3] = strtol(strtok_r(NULL, ".", &endptr),NULL, 10);
    char be[4], le[4];
    //боже ну и ужас, потом надо переделать на фор
    be[0] = (unsigned char)nums[0];
    le[3] = (unsigned char)nums[0];
    be[1] = (unsigned char)nums[1];
    le[2] = (unsigned char)nums[1];
    be[2] = (unsigned char)nums[2];
    le[1] = (unsigned char)nums[2];
    be[3] = (unsigned char)nums[3];
    le[0] = (unsigned char)nums[3];
    for(int i = 0; i < 4; i++){
        printf("nums[X]:%X\tbe[X]:%X\tle[X]:%X\n",nums[i], (unsigned char)be[i], (unsigned char)le[i]);
        printf("nums[s]:%c\tbe[s]:%c\tle[s]:%c\n\n", nums[i], be[i], le[i]);
    }
    int found_flag = 0;
    printf("filename: %s\n",fname);
    FILE *fp = fopen(fname, "rb");
    char buf[4];
    fread(buf, sizeof(char), 4, fp);
    printf("first 4 bytes of file: %X%X%X%X\n", (unsigned char)buf[0],(unsigned char)buf[1],(unsigned char)buf[2],(unsigned char)buf[3]);
    if((buf[0] == be[0] && buf[1] == be[1] && buf[2] == be[2] && buf[3] == be[3]) || (buf[0] == le[0] && buf[1] == le[1] && buf[2] == le[2] && buf[3] == le[3])) return 1;
    while(!feof(fp)){
        char t[1];
        fread(t, sizeof(char), 1, fp);
        for(short i = 0; i < 3; i++) buf[i] = buf[i+1];
        buf[3] = t[0];
        if((buf[0] == be[0] && buf[1] == be[1] && buf[2] == be[2] && buf[3] == be[3]) || (buf[0] == le[0] && buf[1] == le[1] && buf[2] == le[2] && buf[3] == le[3])) return 1;
    }
    return found_flag;
}