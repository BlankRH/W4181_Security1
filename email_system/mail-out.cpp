#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <pwd.h>
#include <cstring>
#include <sys/types.h>

using namespace std;

int count_files(string path) {
    DIR *dp = opendir(path.c_str());
    int cnt = 0;
    struct dirent *ep;

    if (dp != NULL) {
        while ((ep = readdir(dp)))
            if (strncmp(ep->d_name, ".", 1) != 0 && strncmp(ep->d_name, "..", 2) != 0) {
                int num = atoi(ep->d_name);
                if (num > cnt)
                    cnt = num;
            }
        closedir(dp);
    } else {
        exit(3);
    }
    return cnt+1;
    
}

bool validate(string rec_name) {
    for (string::iterator it=rec_name.begin(); it != rec_name.end(); it++) {
        if (*it - '0' < 32 || *it - '0' > 126)
            return 0;
    }
    string path ("mail");
    char buf[1024];
    DIR* dp = opendir(path.c_str());
    struct dirent *ep;
    if (dp != NULL) {
        while ((ep = readdir(dp)))
            if (strncmp(ep->d_name, ".", 1) != 0 && strncmp(ep->d_name, "..", 2) != 0 && rec_name.compare(ep->d_name) == 0) {
                closedir(dp);
                return 1;
            }
    } else {
        exit(3);
    }
    closedir(dp);
    return 0;
}

void printEUser() {
    struct passwd *p;
    uid_t uid;
    if ((p = getpwuid(uid = geteuid())))
    fprintf(stderr, "  pw_name  : %s\n", p->pw_name);
}

int main(int argc, char* argv[]) {
    string rec_name (argv[1]);
    int ok = validate(rec_name);
    if (ok == 0) {
        return 1;
    }
    string path ("mail/" + rec_name);
    int num = count_files(path);
    ostringstream ss;
    ss << setw(5) << setfill('0') << num;
    path += "/" + ss.str();
    ofstream rec_file;
    rec_file.open(path);
    string buffer;
    int cnt = 0;
    while(getline (cin, buffer)) {
        rec_file << buffer << endl;
    }
    rec_file.close();
    if ( ! rec_file ) {
        fprintf(stderr, "%s\n", strerror(errno));
        if (strcmp(strerror(errno), "Permission Denied"))
            return 2;
        return 3;
    }
    /*
    if(acl_set_file(path, acl_type_t type, acl_t acl) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        if (strcmp(strerror(errno), "Permission Denied"))
            return 2;
        return 3;
    }
    */
    return 0;
}