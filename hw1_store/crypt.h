#ifndef _INCLUDE_CRYPT_H_
#define _INCLUDE_CRYPT_H_

#include "aes.h"
#include "sha256.h"
#include <stdio.h>

typedef unsigned char BYTE;            // 8-bit byte
typedef unsigned int WORD;             // 32-bit word, change to "long" for 16-bit machines

void Encrypt();

void Decrypt();

void Validate();


#endif