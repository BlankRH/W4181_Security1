#ifndef _INCLUDE_HANDLER_H_
#define _INCLUDE_HANDLER_H_

#include <stdio.h>
#include <string.h>

void list_handler(const char **argv);
void add_handler(int argc, const char **argv);
void extract_handler(int argc, const char **argv);
void delete_handler(int argc, const char **argv);

#endif