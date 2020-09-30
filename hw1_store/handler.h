#ifndef _INCLUDE_HANDLER_H_
#define _INCLUDE_HANDLER_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>


void init_handler(int argc, const char **argv);
void list_handler(const char **argv);
void add_handler(int argc, const char **argv);
void extract_handler(int argc, const char **argv);
void delete_handler(int argc, const char **argv);

const char *get_pwd(const int, const char**, const char**);
void param_error(const char *option);



#endif