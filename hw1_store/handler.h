#ifndef _INCLUDE_HANDLER_H_
#define _INCLUDE_HANDLER_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

const char *METADATA_PATH = "metadata.txt";

void list_handler(const char **argv);
void add_handler(int argc, const char **argv);
void extract_handler(int argc, const char **argv);
void delete_handler(int argc, const char **argv);

void create_path(char *archive, char *filename, char *file_path);



#endif