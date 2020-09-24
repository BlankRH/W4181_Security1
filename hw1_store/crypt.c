#include "crypt.h"

static const char *RAND_FILE = "/dev/urandom";

void Encrypt(char *filename) {
    FILE *fp = fopen(filename, "r+");
    FILE *rand_fp = fopen(RAND_FILE, "r");

    BYTE key[1][32];
    WORD key_schedule[60];

    fread(key[0], 1, 32, rand_fp);
    
    aes_key_setup(key[0], key_schedule, 256);
}