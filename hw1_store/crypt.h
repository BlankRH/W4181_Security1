#ifndef _INCLUDE_CRYPT_H_
#define _INCLUDE_CRYPT_H_

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

static const char *METADATA_PATH = "metadata.txt";
static const char *CODE_PATH = "hashcode.txt";

typedef unsigned char BYTE;            // 8-bit byte
typedef unsigned int WORD;             // 32-bit word, change to "long" for 16-bit machines

#define KEY_SIZE 32
static const int KEY_SCHEDULE_LEN = 256;
static const int BUF_SIZE = 1024;

void Encrypt(char *read_path, char *write_path, const BYTE key[]);

void Decrypt(char *read_path, char *write_path, const BYTE key[]);

void Validate(const char *archive, BYTE key[]);

void Hash(const char text[], BYTE buf[], int iter_time);

void HMAC(BYTE key[], const char *archive, BYTE buf[]);

void create_path(const char *archive, const char *filename, char *file_path);

int check_file(const char *file, BYTE flag);

void renew_metadata(const char *archive);

#endif