#include "cstore.h"
#include "handler.h"

void cmd_paraphase(int argc, const char **argv) {

    if (strcmp(argv[1], "list") == 0) {
        if (argc != 3) {
            param_error("list");
        } else {
            list_handler(argv);
        }
    } else if (strcmp(argv[1], "add") == 0) {
        if(argc < 4) {
            param_error("add");
        } else {
            add_handler(argc, argv);
        }
    } else if (strcmp(argv[1], "extract") == 0) {
        if(argc < 4) {
            param_error("extract");
        } else {
            extract_handler(argc, argv);
        }
    } else if (strcmp(argv[1], "delete") == 0) {
        if(argc < 4) {
            param_error("delete");
        } else {
            extract_handler(argc, argv);
        }
    } else {
        fprintf(2, "<Usage>: cstore <option> [-p password] <archivename> [files]\n");
        exit(1);
    }

}

int main(int argc, const char* argv[]) {

    if(argc < 4) {
        fprintf(2, "<Usage>: cstore <option> [-p password] <archivename> <file>\n");
        exit(1);
    }

    cmd_paraphase(argc, argv);

    return 0;

}

const char* get_pwd(const int argc, const char **argv) {
    if (strcmp(argv[0], "-p") != 0) {
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

const char* get_archive_name(const char **params) {
    return params[2];
}

const char** get_files_name(const char **params) {
    return params[3];
}

void param_error(const char *option) {
    if(option[0] == 'l') {
        fprintf(2, "<Usage> cstore list <archievement> <files>\n");
        exit();
    } else {
        fprintf(2, "<Usage> cstore %s [-p password] <archievement> <files>\n", option);
        exit();
    }
}