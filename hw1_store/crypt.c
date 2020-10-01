#include "crypt.h"
#include "sha256.h"
#include "my_aes.h"

static const char *RAND_FILE = "/dev/urandom";

void create_path(const char *archive, const char *filename, char *file_path) {
    strcpy(file_path, archive);
    strcat(file_path, "/");
    strcat(file_path, filename);
}

void Encrypt(char *read_path, char *write_path, const BYTE key[]) {

    printf("Encrypting...\n");

    FILE *in = fopen(read_path, "rb");
    FILE *rand_fp = fopen(RAND_FILE, "rb");
    FILE *out = fopen(write_path, "wb");

    WORD key_schedule[60];

    BYTE iv[AES_BLOCK_SIZE];

    fread(iv, 1, AES_BLOCK_SIZE, rand_fp);
    fwrite(iv, 1, AES_BLOCK_SIZE, out);
    
    aes_key_setup(key, key_schedule, KEY_SCHEDULE_LEN);

    // pkcs5 padding
    my_aes_encrypt_cbc(in, out, key_schedule, KEY_SCHEDULE_LEN, iv);

    fclose(in);
    fclose(rand_fp);
    fclose(out);

    printf("Encryption Finished\n");
}

void Decrypt(char *read_path, char *write_path, const BYTE key[]) {

    printf("Decrypting...\n");
    FILE *tmp_in = fopen(read_path, "rb");
    fseek(tmp_in, 0, SEEK_END);
    int in_size = ftell(tmp_in);
    fclose(tmp_in);
    
    FILE *in = fopen(read_path, "rb");
    FILE *out = fopen(write_path, "wb");

    WORD key_schedule[60];

    BYTE iv[AES_BLOCK_SIZE];
    
    aes_key_setup(key, key_schedule, KEY_SCHEDULE_LEN);

    fread(iv, 1, AES_BLOCK_SIZE, in);

    // pkcs5 padding
    my_aes_decrypt_cbc(in, in_size, out, key_schedule, KEY_SCHEDULE_LEN, iv);

    fclose(in);
    fclose(out);

    printf("Decryption Finished\n");
}

void Hash(const char text[], BYTE buf[], int iter_time) {
    SHA256_CTX ctx;
    sha256_init(&ctx);
    for (int i = 0; i < iter_time; ++i)
	   sha256_update(&ctx, text, strlen(text));
	sha256_final(&ctx, buf);
}

void HMAC(BYTE key[], const char *archive, BYTE buf[]) {
    printf("Generating new hashcode...\n");
    struct dirent *dp = NULL;
    SHA256_CTX ctx;

    DIR *d = opendir(archive);
    if (d == NULL) {
        fprintf(stderr, "c opendir failed\n\n");
        exit(1);
    }
    
    sha256_init(&ctx);
    sha256_update(&ctx, key, SHA256_BLOCK_SIZE);
    while((dp = readdir(d)) != NULL) {
        if((!strncmp(dp->d_name, ".", 1)) || (!strncmp(dp->d_name, "..", 2))||(!strncmp(dp->d_name, "hashcode.txt", 12)))
            continue;
        char filepath[BUF_SIZE];
        BYTE text[BUF_SIZE];
        create_path(archive, dp->d_name, filepath);
        sha256_update(&ctx, dp->d_name, strlen(dp->d_name));
        FILE *fp = fopen(filepath, "rb");
        size_t tlen;
        while(tlen = fread(text, 1, BUF_SIZE, fp)) {
            sha256_update(&ctx, text, tlen);
            memset(text, 0, BUF_SIZE);
        }
        fclose(fp);
    }
    closedir(d);
    sha256_final(&ctx, buf);

    printf("New Hashcode Generated\n");
    
}

void Validate(const char *archive, BYTE key[]) {
    printf("Validating...\n");

    if(access(archive, F_OK) != 0) {
        fprintf(stderr, "archive does not exist\n\n");
        exit(1);
    }

    renew_metadata(archive);

    BYTE code1[SHA256_BLOCK_SIZE];
    BYTE code2[SHA256_BLOCK_SIZE];

    HMAC(key, archive, code2);

    char mpath[BUF_SIZE];
    create_path(archive, CODE_PATH, mpath);
    if(!check_file(mpath, 0x01)) {
        fprintf(stderr, "Authentication fail\n\n");
        exit(1);
    }
    FILE *fp = fopen(mpath, "rb");
    

    fread(code1, 1, SHA256_BLOCK_SIZE, fp);
    fclose(fp);

    if(memcmp(code1, code2, 32) != 0) {
        fprintf(stderr, "Authentication fail\n\n");
        exit(1);
    } else {
        printf("Integrity Check & Authentication Success\n");
    }

    printf("Validation Finished\n");
}

int check_file(const char *file, BYTE flag) {
    // flag:
    // bit 0: existence
    // bit 1: 0file / 1archive
    struct stat st;
    stat(file, &st);
    int exist = access(file, F_OK);

    if(exist == -1) {
        if(flag & 0x01) {
            fprintf(stderr, "%s Does Not Exist\n\n", file);
            return 0;
        }
    } else if (exist == 0) {
        if(!(flag & 0x01)) {
            fprintf(stderr, "%s Already Exists\n\n", file);
            return 0;
        } else {
            if(S_ISDIR(st.st_mode) && !(flag & 0x02)) {
                fprintf(stderr, "%s Is Archive\n\n", file);
                return 0;
            }
            if(!S_ISDIR(st.st_mode) && (flag & 0x02)) {
                fprintf(stderr, "%s Is File\n\n", file);
                return 0;
            }
        }
    } else {
        fprintf(stderr, "c access function error\n\n");
        exit(1);
    }
    return 1;

}

void renew_metadata(const char *archive) {

    char mpath[BUF_SIZE];
    create_path(archive, METADATA_PATH, mpath);
    if(!check_file(mpath, 0x1)) {
        fprintf(stderr, "Missing metadata.txt\n\n");
        exit(1);
    }
    remove(mpath);
    FILE *fp = fopen(mpath, "w");

    struct dirent *dp = NULL;
    DIR *d = opendir(archive);
    if (d == NULL) {
        fprintf(stderr, "c opendir failed\n\n");
        exit(1);
    }
    while((dp = readdir(d)) != NULL) {
        if((!strncmp(dp->d_name, ".", 1)) || (!strncmp(dp->d_name, "..", 2)))
            continue;
        fputs(dp->d_name, fp);
        fputs("\n", fp);
    }

    closedir(d);
    fclose(fp);
}