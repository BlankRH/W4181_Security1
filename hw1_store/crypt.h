#ifndef _INCLUDE_CRYPT_H_
#define _INCLUDE_CRYPT_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

static const char *METADATA_PATH = "metadata.txt";
static const char *CODE_PATH = "hashcode.txt";

typedef unsigned char BYTE;            // 8-bit byte
typedef unsigned int WORD;             // 32-bit word, change to "long" for 16-bit machines

static const int KEY_SIZE = 32;
static const int KEY_SCHEDULE_LEN = 256;
static const int BUF_SIZE = 1024;

void Encrypt(char *read_path, char *write_path, const BYTE key[]);

void Decrypt(char *read_path, char *write_path, const BYTE key[]);

void Validate();

void Hash();

void HMAC();

void create_path(const char *archive, const char *filename, char *file_path);


#endif