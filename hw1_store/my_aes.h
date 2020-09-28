#ifndef _INCLUDE_MY_AES_H_
#define _INCLUDE_MY_AES_H_

#include <stddef.h>

#define AES_BLOCK_SIZE 16

typedef unsigned char BYTE;
typedef unsigned int WORD;

// TAKEN FROM AES.H

void aes_key_setup(const BYTE key[],          // The key, must be 128, 192, or 256 bits
                   WORD w[],                  // Output key schedule to be used later
                   int keysize);              // Bit length of the key, 128, 192, or 256

void aes_encrypt(const BYTE in[],             // 16 bytes of plaintext
                 BYTE out[],                  // 16 bytes of ciphertext
                 const WORD key[],            // From the key setup
                 int keysize);                // Bit length of the key, 128, 192, or 256

void aes_decrypt(const BYTE in[],             // 16 bytes of ciphertext
                 BYTE out[],                  // 16 bytes of plaintext
                 const WORD key[],            // From the key setup
                 int keysize);                // Bit length of the key, 128, 192, or 256

// END

int aes_encrypt_cbc(FILE *in, FILE *out, const WORD key[], int keysize, const BYTE iv[]);

int aes_encrypt_cbc_mac(FILE *in, FILE *out, const WORD key[], int keysize, const BYTE iv[]);


#endif