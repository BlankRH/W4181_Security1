#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;

int count_files(string path) {
    auto dirIter = std::filesystem::directory_iterator(path);
    int fileCount = 0;

    for (auto& entry : dirIter)
    {
        if (entry.is_regular_file())
        {
            ++fileCount;
        }
    }
    return fileCount;
}

int main(int argc, char* argv[]) {
    int a, b;
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments\n");
        return -1;
    }
    string rec_name (argv[0]);
    string path ("../mail/" + rec_name);
    int num = count_files(path);
    path += "/" + to_string(num);
    fstream rec_file;
    if (rec_file.open(path, fstream::out))
    string buffer;
    while(getline(cin, buffer)) {
        rec_file << buffer << endl;
    }
    return 0;
}