#include "handler.h"
#include "stdlib.h"

void cmd_paraphase(int argc, const char **argv) {

    if (strcmp(argv[1], "list") == 0) {
        if (argc != 3) {
            param_error("list");
        } else {
            list_handler(argv);
        }
    } else if (strcmp(argv[1], "init") == 0) {
        if (argc != 3) {
            param_error("init");
        } else {
            init_handler(argc, argv);
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
        fprintf(stderr, "<Usage>: cstore <option> [-p password] <archivename> [files]\n");
        exit(1);
    }

}

int main(int argc, const char* argv[]) {

    if(argc < 3) {
        fprintf(stderr, "<Usage>: cstore <option> [-p password] <archivename> <file>\n");
        exit(1);
    }

    cmd_paraphase(argc, argv);

    return 0;

}


