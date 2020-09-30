#include "handler.h"
#include "crypt.h"
#include "sha256.h"



void list_handler(const char **argv) {
    const char *archive = argv[2];
    char file_path[BUF_SIZE];

    if(!check_file(archive, 0x3))
        exit(1);

    create_path(archive, METADATA_PATH, file_path);

    FILE *fp = fopen(file_path, "a+");

    char buf[BUF_SIZE];
    printf("Printing filenames in archive...\n");
    while(fgets(buf, BUF_SIZE, fp)) {
        printf("%s", buf);
    }
    fclose(fp);
}

void init_handler(int argc, const char **argv) {

    printf("Initializing Archive...\n");
    
    const char *archive = NULL;
    const char *password = get_pwd(argc, argv, &archive);
    int archiveidx;

    if(password == NULL) {
        archiveidx = 2;
        password = getpass("Enter password:");
    } else {
        archiveidx = 4;
    }

    BYTE key[KEY_SIZE];
    Hash(password, key);

    for(int i=archiveidx; i<argc; i++) {

        archive = argv[i];
        
        if(!check_file(archive, 0x2))
            continue;
        
        mkdir(archive, 0777);

        char mpath[BUF_SIZE];
        create_path(archive, METADATA_PATH, mpath);
        FILE *mfp = fopen(mpath, "wb");

        char cpath[BUF_SIZE];
        create_path(archive, CODE_PATH, cpath);
        FILE *cfp = fopen(cpath, "wb");

        struct dirent *dp = NULL;
        DIR *d = opendir(archive);
        if (d == NULL) {
            fprintf(stderr, "c opendir failed\n");
            exit(1);
        }
        while((dp = readdir(d)) != NULL) {
            if((!strncmp(dp->d_name, ".", 1)) || (!strncmp(dp->d_name, "..", 2)))
                continue;
            fputs(dp->d_name, mfp);
            fputs("\n", mfp);
        }
        fclose(mfp);

        BYTE code[SHA256_BLOCK_SIZE]; 
        HMAC(key, archive, code);

        fwrite(code, 1, 32, cfp);
        fclose(cfp);
    }
}

void add_handler(int argc, const char **argv) {

    const char *archive = NULL;
    const char *password = get_pwd(argc, argv, &archive);
    int archiveidx;

    if(!check_file(archive, 0x3))
        exit(1);

    if(password == NULL) {
        archiveidx = 2;
        password = getpass("Enter password:");
    } else {
        archiveidx = 4;
    }

    BYTE key[KEY_SIZE];

    Hash(password, key);

    Validate(archive, key);

    char mpath[BUF_SIZE];
    create_path(archive, METADATA_PATH, mpath);
    FILE *fp = fopen(mpath, "a");

    BYTE code[SHA256_BLOCK_SIZE];

    for(int i=archiveidx+1; i<argc; i++) {
        char rpath[BUF_SIZE];
        char wpath[BUF_SIZE];
        const char *filename = argv[i];
        strcpy(rpath, filename);
        create_path(archive, filename, wpath);
        if(!check_file(wpath, 0x00)) {
            continue;
        }
        if(access(rpath, F_OK) == 0) {
            Encrypt(rpath, wpath, key);
        } else {
            FILE *tmp = fopen(wpath, "w");
            fclose(tmp);
        }
        fputs(filename, fp);
        fputs("\n", fp);
    }

    fclose(fp);
    
    HMAC(key, archive, code);
    char cpath[BUF_SIZE];
    create_path(archive, CODE_PATH, cpath);
    FILE *newfp = fopen(cpath, "wb");
    fwrite(code, 1, 32, newfp);
    fclose(newfp);
    
}
void extract_handler(int argc, const char **argv) {
    const char *archive = NULL;
    const char *password = get_pwd(argc, argv, &archive);
    int archiveidx;

    if(!check_file(archive, 0x3)) {
        exit(1);
    }

    if(password == NULL) {
        archiveidx = 2;
        password = getpass("Enter password:");
    } else {
        archiveidx = 4;
    }

    BYTE key[KEY_SIZE];

    Hash(password, key);

    Validate(archive, key);

    for(int i=archiveidx+1; i<argc; i++) {
        
        char rpath[BUF_SIZE];
        char wpath[BUF_SIZE];
        const char *filename = argv[i];
        strcpy(wpath, filename);
        create_path(archive, argv[i], rpath);
        if(!check_file(rpath, 0x1)) 
            continue;
        if((!strncmp(argv[i], "hashcode.txt", 12)) || (!strncmp(argv[i], "metadata.txt", 12))) {
            char buf[BUF_SIZE];
            size_t read_size;
            FILE *out = fopen(wpath, "wb");
            FILE *in = fopen(rpath, "rb");
            while(read_size = fread(buf, 1, BUF_SIZE, in)) {
                fwrite(buf, 1, read_size, out);
            }
            fclose(out);
            fclose(in);
        } else {
            Decrypt(rpath, wpath, key);
        }
    }

}
void delete_handler(int argc, const char **argv) {

    const char *archive = NULL;
    const char *password = get_pwd(argc, argv, &archive);
    int archiveidx;

    if(!check_file(archive, 0x3))
        exit(1);

    if(password == NULL) {
        archiveidx = 2;
        password = getpass("Enter password:");
    } else {
        archiveidx = 4;
    }

    BYTE key[KEY_SIZE];

    Hash(password, key);

    Validate(archive, key);

    for(int i=archiveidx+1; i<argc; i++) {
        
        char path[BUF_SIZE];
        const char *filename = argv[i];
        create_path(archive, filename, path);
        if(!check_file(path, 0x1)) {
            continue;
        }
        printf("Removing file %s from archive %s...\n", filename, archive);
        remove(path);
    }

    char mpath[BUF_SIZE];
    create_path(archive, METADATA_PATH, mpath);
    remove(mpath);
    FILE *fp = fopen(mpath, "w");

    struct dirent *dp = NULL;
    DIR *d = opendir(archive);
    if (d == NULL) {
        fprintf(stderr, "c opendir failed\n");
        exit(1);
    }
    while((dp = readdir(d)) != NULL) {
        if((!strncmp(dp->d_name, ".", 1)) || (!strncmp(dp->d_name, "..", 2)))
            continue;
        fputs(dp->d_name, fp);
        fputs("\n", fp);
    }

    fclose(fp);

    BYTE code[SHA256_BLOCK_SIZE];

    HMAC(key, archive, code);

    char cpath[BUF_SIZE];
    create_path(archive, CODE_PATH, cpath);
    FILE *newfp = fopen(cpath, "wb");
    fwrite(code, 1, 32, newfp);
    fclose(newfp);
}


const char* get_pwd(const int argc, const char **argv, const char **archive) {
    if (strcmp(argv[2], "-p") != 0) {
        *archive = argv[2];
        return NULL;
    } else {
        *archive = argv[4];
        return argv[3];
    }
}

void param_error(const char *option) {
    if(option[0] == 'l') {
        fprintf(stderr, "<Usage> cstore %s <archive>\n", option);
    } else if(option[0] == 'i') {
        fprintf(stderr, "<Usage> cstore %s [-p password] <archive>\n", option);
    } else {
        fprintf(stderr, "<Usage> cstore %s [-p password] <archive> <files>\n", option);
    }
    exit(1);
}

