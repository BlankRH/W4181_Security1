#include "my_aes.h"
#include "aes.h"

void xor(const BYTE in[], BYTE out[], size_t len) {
    for(size_t i=0; i<len; i++) {
        out[i] ^= in[i];
    }

}


// the following is implemented by myself
int my_aes_encrypt_cbc(FILE *in, FILE *out, const WORD key[], int keysize, const BYTE iv[]) {
    BYTE buf_in[AES_BLOCK_SIZE], buf_out[AES_BLOCK_SIZE], buf_iv[AES_BLOCK_SIZE];

    memcpy(buf_iv, iv, AES_BLOCK_SIZE);
    size_t read_size;
    int pad = 0;

    while(read_size = fread(buf_in, 1, AES_BLOCK_SIZE, in)) {
        if(read_size < AES_BLOCK_SIZE) {
            int fill_byte = AES_BLOCK_SIZE - read_size;
            memset(buf_in+read_size, fill_byte, AES_BLOCK_SIZE);
            pad = 1;
        }
        xor(buf_iv, buf_in, AES_BLOCK_SIZE);
        aes_encrypt(buf_in, buf_out, key, keysize);
        memcpy(buf_iv, buf_out, AES_BLOCK_SIZE);
        fwrite(buf_out, 1, AES_BLOCK_SIZE, out);
    }
    if (!pad) {
        memset(buf_in, AES_BLOCK_SIZE, AES_BLOCK_SIZE);
        xor(buf_iv, buf_in, AES_BLOCK_SIZE);
        aes_encrypt(buf_in, buf_out, key, keysize);
        fwrite(buf_out, 1, AES_BLOCK_SIZE, out);
    }

    return 1;
}

int my_aes_decrypt_cbc(FILE *in, FILE *out, const WORD key[], int keysize, const BYTE iv[]) {
    BYTE buf_in[AES_BLOCK_SIZE], buf_out[AES_BLOCK_SIZE], buf_iv[AES_BLOCK_SIZE];

    memcpy(buf_iv, iv, AES_BLOCK_SIZE);

    while(fread(buf_in, 1, AES_BLOCK_SIZE, in)) {
        aes_decrypt(buf_in, buf_out, key, keysize);
        xor(buf_iv, buf_out, AES_BLOCK_SIZE);
        memcpy(buf_iv, buf_in, AES_BLOCK_SIZE);
        fwrite(buf_out, 1, AES_BLOCK_SIZE, out);
    }

    return 1;

}