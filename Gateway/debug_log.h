#ifndef __DEBUG_LOG__
#define __DEBUG_LOG__

#include"str.h"

#define DEBUG_STR(pc_str) {\
    puts("File:");\
    puts(__FILE__);\
    puts("\nfunction:");\
    puts(__FUNCTION__);\
    puts("\n");\
    puts("msg:");\
    puts(pc_str);\
    puts("\n");\
}

#define DEBUG_INT(pc_str) {\
    puts("File:");\
    puts(__FILE__);\
    puts("\nfunction:");\
    puts(__FUNCTION__);\
    puts("\n");\
    puts("msg:");\
    puts(xtoa(pc_str));\
    puts("\n");\
}

#endif