#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <sstream>
#include <iomanip>
#include <cstring>

using namespace std;

int count_files(string path) {
    DIR *dp = opendir(path.c_str());
    int cnt = 0;
    struct dirent *ep;

    if (dp != NULL) {
        while ((ep = readdir(dp)))
            if (strncmp(ep->d_name, ".", 1) != 0 && strncmp(ep->d_name, "..", 2) != 0)
                cnt++;
        closedir(dp);
    } else {
        exit(2);
    }
    return cnt+1;
    
}

bool validate(string rec_name) {
    string path ("mail");
    DIR* dp = opendir(path.c_str());
    struct dirent *ep;
    if (dp != NULL) {
        while ((ep = readdir(dp)))
            if (strncmp(ep->d_name, ".", 1) != 0 && strncmp(ep->d_name, "..", 2) != 0 && rec_name.compare(ep->d_name) == 0) {
                closedir(dp);
                return 1;
            }
    } else {
        exit(2);
    }
    closedir(dp);
    return 0;
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
    fstream rec_file;
    rec_file.open(path, fstream::out);
    string buffer;
    while(getline (cin, buffer)) {
        if (buffer.find(EOF) != string::npos) {
            break;
        }
        rec_file << buffer << endl;
    }
    return 0;
}