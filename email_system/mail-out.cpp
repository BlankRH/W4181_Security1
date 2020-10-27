#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <sstream>
#include <iomanip>
#include <cstring>

using namespace std;

int count_files(string path) {
    DIR *dp;
    int cnt = 0;
    struct dirent *ep;
    dp = opendir(path.c_str());

    if (dp != NULL) {
        while ((ep = readdir(dp)))
            if (strncmp(ep->d_name, ".", 1) != 0 && strncmp(ep->d_name, "..", 2) != 0)
                cnt++;
        closedir(dp);
    } else {
        exit(2);
    }
    return cnt;
    
}

bool validate(string path) {
    DIR* dir = opendir(path.c_str());
    if (dir) {
        closedir(dir);
        return 1;
    } else if (ENOENT == errno){
        return 0;
    } else {
        return -1;
    }
}

int main(int argc, char* argv[]) {
    string rec_name (argv[1]);
    string path ("mail/" + rec_name);
    int ok = validate(path);
    if (ok == 0) {
        return 1; 
    } else if (ok == -1) {
        return 2;
    }
    int num = count_files(path);
    ostringstream ss;
    ss << setw(5) << setfill('0') << num;
    path += "/" + ss.str();
    fstream rec_file;
    rec_file.open(path, fstream::out);
    string buffer;
    while(getline (cin, buffer)) {
        if (buffer.find(EOF) != string::npos)
            break;
        rec_file << buffer << endl;
    }
    return 0;
}