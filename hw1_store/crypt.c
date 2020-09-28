#include "crypt.h"
#include "sha256.h"
#include "handler.h"
#include "my_aes.h"

void Encrypt(char *read_path, char *write_path, const BYTE key[]) {

    FILE *in = fopen(read_path, "rb");
    FILE *rand_fp = fopen(RAND_FILE, "rb");
    FILE *out = fopen(write_path, "wb");

    WORD key_schedule[60];

    BYTE iv[AES_BLOCK_SIZE];

    fread(iv, 1, AES_BLOCK_SIZE, rand_fp);
    fwrite(iv, 1, AES_BLOCK_SIZE, out);
    
    aes_key_setup(key, key_schedule, KEY_SCHEDULE_LEN);

    // pkcs5 padding
    aes_encrypt_cbc(in, out, key_schedule, KEY_SCHEDULE_LEN, iv);

    fclose(in);
    fclose(rand_fp);
    fclose(out);

}

void Decrypt(char *read_path, char *write_path, const BYTE key[]) {
    
    FILE *in = fopen(read_path, "rb");
    FILE *out = fopen(write_path, "wb");

    WORD key_schedule[60];

    BYTE iv[AES_BLOCK_SIZE];
    
    aes_key_setup(key, key_schedule, KEY_SCHEDULE_LEN);

    fread(iv, 1, AES_BLOCK_SIZE, in);

    // pkcs5 padding
    aes_decrypt_cbc(in, out, key_schedule, KEY_SCHEDULE_LEN, iv);

    fclose(in);
    fclose(out);
}

void Hash(BYTE text[], BYTE buf[]) {
    SHA256_CTX ctx;
    sha256_init(&ctx);
    for (int i = 0; i < 100000; ++i)
	   sha256_update(&ctx, text, strlen(text));
	sha256_final(&ctx, buf);
}

void HMAC(BYTE key[], char *archive, BYTE buf[]) {
    struct dirent *dp = NULL;
    DIR *d = NULL;
    SHA256_CTX ctx;
    
    sha256_init(&ctx);
    sha256_update(&ctx, key, strlen(key));
    while((dp = readdir(d)) != NULL) {
        if((!strncmp(dp->d_name, ".", 1)) || (!strncmp(dp->d_name, "..", 2)))
            continue;
        char filepath[BUF_SIZE];
        BYTE text[BUF_SIZE];
        create_path(archive, dp->d_name, filepath);
        FILE *fp = fopen(filepath, "rb");
        while(fread(text, 1, BUF_SIZE, fp)) {
            sha256_update(&ctx, text, strlen(text));
            memset(text, 0, BUF_SIZE);
        }
    }
    sha256_final(&ctx, buf);
    
}

void Validate(char *archive, BYTE key[]) {
    BYTE code1[SHA256_BLOCK_SIZE];
    BYTE code2[SHA256_BLOCK_SIZE];

    HMAC(key, archive, code2);
    char mpath[BUF_SIZE];
    create_path(archive, METADATA_PATH, mpath);
    FILE *fp = fopen(mpath, "rb");
    fread(code1, 1, SHA256_BLOCK_SIZE, fp);
    if(memcmp(code1, code2, 32) != 0) {
        fprintf(2, "Authentication fail\n");
        exit(1);
    }
}