#ifndef _INCLUDE_MY_AES_H_
#define _INCLUDE_MY_AES_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "aes.h"

#define AES_BLOCK_SIZE 16

typedef unsigned char BYTE;
typedef unsigned int WORD;

int my_aes_encrypt_cbc(FILE *in, FILE *out, const WORD key[], int keysize, const BYTE iv[]);

int my_aes_decrypt_cbc(FILE *in, int in_size, FILE *out, const WORD key[], int keysize, const BYTE iv[]);


#endif