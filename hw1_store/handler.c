#include "handler.h"
#include "crypt.h"



void create_path(char *archive, char *filename, char *file_path) {
    strcpy(file_path, archive);
    strcat(file_path, "/");
    strcat(file_path, filename);
}


void list_handler(const char **argv) {
    char *archive = argv[2];
    char file_path[BUF_SIZE];

    create_path(archive, METADATA_PATH, file_path);

    FILE *fp = fopen(file_path, "r");

    char buf[BUF_SIZE];
    printf("Printing filenames in archive...");
    while(read(fp, buf, BUF_SIZE)) {
        printf(buf);
    }

}

void init_handler(int argc, const char **argv) {
    char *archive = argv[2];
    char *password = get_pwd(argc, argv);
    int archiveidx;
    BYTE key[KEY_SIZE];
    BYTE code[SHA256_BLOCK_SIZE];

    if(password == NULL) {
        archiveidx = 4;
        password = getpass("Enter password:");
    } else {
        archiveidx = 2;
    }
    if(access(archive, F_OK) == 0) {
        fprintf(2, "Archive Alraeady Exists");
        exit(0);
    }
    
    Hash(password, key);
    mkdir(archive, 0777);

    HMAC(key, archive, code);

    char mpath[BUF_SIZE];
    create_path(archive, METADATA_PATH, mpath);
    FILE *fp = fopen(mpath, "wb");
    fprintf(code, 1, 32, fp);
    fclose(fp);
}

void add_handler(int argc, const char **argv) {
    char *archive;
    char *password = get_pwd(argc, argv);
    int archiveidx;
    BYTE key[KEY_SIZE];
    BYTE code[SHA256_BLOCK_SIZE];

    if(password == NULL) {
        archiveidx = 4;
        password = getpass("Enter password:");
    } else {
        archiveidx = 2;
    }

    Hash(password, key);

    archive = argv[archiveidx];

    Validate(archive, key);

    char mpath[BUF_SIZE];
    create_path(archive, METADATA_PATH, mpath);
    FILE *fp = fopen(mpath, "a");

    for(int i=archiveidx+1; i<argc; i++) {
        char rpath[BUF_SIZE];
        char wpath[BUF_SIZE];
        char *filename = argv[i];
        strcpy(rpath, filename);
        create_path(archive, filename, wpath);
        if(access(wpath, F_OK) == 0) {
            remove(wpath);
        }
        Encrypt(rpath, wpath, key);
        fprintf(fp, "%s\n", filename);
    }

    fclose(fp);
    
    HMAC(key, archive, code);

    FILE *newfp = fopen(mpath, "wb");
    fprintf(code, 1, 32, newfp);
    fclose(newfp);
    
}
void extract_handler(int argc, const char **argv) {
    char *archive;
    char *password = get_pwd(argc, argv);
    int archiveidx;
    char key[KEY_SIZE];

    if(password == NULL) {
        archiveidx = 4;
        password = getpass("Enter password:");
    } else {
        archiveidx = 2;
    }

    generate_key(password, key);

    archive = argv[archiveidx];

    Validate(archive);

    for(int i=archiveidx+1; i<argc; i++) {
        char rpath[BUF_SIZE];
        char wpath[BUF_SIZE];
        strcpy(rpath, argv[i]);
        create_path(archive, argv[i], wpath);
        if(access(rpath, F_OK) == -1) {
            fprintf(2, "Error: file not exist");
            exit(1);
        }
        Decrypt(rpath, wpath, key);
    }

}
void delete_handler(int argc, const char **argv) {

    char *archive;
    char *password = get_pwd(argc, argv);
    int archiveidx;
    BYTE key[KEY_SIZE];
    BYTE code[SHA256_BLOCK_SIZE];

    if(password == NULL) {
        archiveidx = 4;
        password = getpass("Enter password:");
    } else {
        archiveidx = 2;
    }

    generate_key(password, key);

    archive = argv[archiveidx];

    Validate(archive);

    for(int i=archiveidx+1; i<argc; i++) {
        char path[BUF_SIZE];
        strcpy(path, argv[i]);
        create_path(archive, argv[i], path);
        if(access(path, F_OK) == -1) {
            fprintf(2, "Error: file not exist");
            exit(1);
        }
        remove(path);
    }

    char mpath[BUF_SIZE];
    create_path(archive, METADATA_PATH, mpath);
    remove(mpath);
    FILE *fp = fopen(mpath, "w");

    struct dirent *dp = NULL;
    DIR *d = NULL;
    while((dp = readdir(d)) != NULL) {
        if((!strncmp(dp->d_name, ".", 1)) || (!strncmp(dp->d_name, "..", 2)))
            continue;
        fprintf(mpath, "%s\n", dp->d_name);
    }

    HMAC(key, archive, code);

    FILE *newfp = fopen(mpath, "wb");
    fprintf(code, 1, 32, newfp);
    fclose(newfp);
}


const char* get_pwd(const int argc, const char **argv) {
    if (strcmp(argv[2], "-p") != 0) {
        return NULL;
    } else {
        if(argc < 6) {
            param_error(argv[1]);
            exit(1);
        } else {
            return argv[3];
        }
    }
}