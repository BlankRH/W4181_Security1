#ifndef _INCLUDE_CSTORE_H_
#define _INCLUDE_CSTORE_H_

#include <stdio.h>
#include <string.h>

const char* get_pwd(const int argc, const char **argv);
const char* get_archive_name(const char **params);
const char** get_files_name(const char **params);

void cmd_paraphase(int argc, const char **argv);

void param_error(const char *option);


#endif