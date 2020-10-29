#include <regex>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <sys/wait.h>
#include <cctype>
#include <set>
#include <dirent.h>

using namespace std;

string whitespaces(" \t\f\v\n\r");

bool validate(string);
void invoke(string, string);

void split() {
    string buf;
    int step = 0;
    string sender;
    string rcpt_line;
    set<string> receivers;
    string message;
    while (getline (cin, buf)) {
        if (buf.compare(".") == 0) {
            if(step == -1) {
                fprintf(stderr, "Aborted Mail\n");
            } else if (step == 1) {
                fprintf(stderr, "Err: No RCPT TO or No DATA\n");
            } else if (step == 0) {
                fprintf(stderr, "Err: No MAIL FROM; Reaching end\n");
            } else {
                message += EOF;
                message += '\n';
                for(auto it=receivers.begin(); it != receivers.end(); it++) {
                    string rec (*it);
                    invoke(rec, message);
                }
            }
            message.clear();
            receivers.clear();
            step = 0;
            continue;
        }
        if (step == -1) {
            continue;
        }
        if (message.size() > 1024 * 1024 * 1024) {
            fprintf(stderr, "Err: Message Too Long\n");
            step = -1;
            continue;
        }
        if (buf.compare(0, 1, ".") == 0) {
            buf.erase(0,1);
        }
        if (step == 0) {
            string cmpbuf (buf);
            transform(cmpbuf.begin(), cmpbuf.end(), cmpbuf.begin(), ::tolower);
            if (cmpbuf.compare(0, 10, "mail from:") != 0) {
                fprintf(stderr, "Err: No MAIL FROM; Jumping to end\n");
                step = -1;
            } else if (buf.compare(10, 1, "<") != 0 || buf.find_last_of(">") != buf.find_last_not_of(whitespaces)){
                fprintf(stderr, "Err: Incorrect angle brackets; Jumping to end\n");
                step = -1;
            } else {
                string username = buf.substr(11, buf.find_last_of(">")-11);
                bool nameOK = validate(username);
                if (!nameOK) {
                    fprintf(stderr, "Err: Bad Sender Username %s; Jumping to end\n", username.c_str());
                    step = -1;
                } else {
                    sender = "From: " + username;
                    step++;
                }
            }
        } else if (step == 1) {
            string cmpbuf (buf);
            transform(cmpbuf.begin(), cmpbuf.end(), cmpbuf.begin(), ::tolower);
            if (cmpbuf.compare(0, 8, "rcpt to:") != 0 && receivers.empty()) {
                fprintf(stderr, "Err: No RCPT TO; Jumping to end\n");
                step = -1;
            } else if (cmpbuf.compare(0, 8, "rcpt to:") == 0) {
                if (buf.compare(8, 1, "<") != 0 || buf.find_last_of(">") != buf.find_last_not_of(whitespaces)){
                    fprintf(stderr, "Err: Incorrect angle brackets\n");
                    continue;
                } 
                string username = buf.substr(9, buf.find_last_of(">")-9);
                receivers.insert(username);
            } else if (cmpbuf.compare("data") == 0) {
                rcpt_line = "    To: ";
                for(auto it=receivers.begin(); it != receivers.end(); it++) {
                    rcpt_line += *it + ", ";
                    if (rcpt_line.size() > 1024 * 1024 * 1024) {
                        fprintf(stderr, "Err: Message Too Long\n");
                        step = -1;
                        break;
                    }
                }
                if (step == -1) {
                    continue;
                }
                if (receivers.size() > 0) {
                    rcpt_line.pop_back();
                    rcpt_line.pop_back();
                }
                message = sender + "\n" + rcpt_line + "\n" + "\n";
                sender.clear();
                rcpt_line.clear();
                step++;
            } else {
                fprintf(stderr, "Err: Invalid Control Line; Jumping to end\n");
                step = -1;
            }
        } else if (step == 2) {
            message += buf + "\n";
        }
    }

}

bool validate(string username) {
    string path ("mail");
    DIR* dp = opendir(path.c_str());
    struct dirent *ep;
    if (dp != NULL) {
        while ((ep = readdir(dp)))
            if (strncmp(ep->d_name, ".", 1) != 0 && strncmp(ep->d_name, "..", 2) != 0 && username.compare(ep->d_name) == 0) {
                closedir(dp);
                return 1;
            }
    } else {
        exit(2);
    }
    closedir(dp);
    return 0;
}


void invoke(string rec, string buffer) {
    int fd[2];
    pid_t p;

    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe Failed\n");
        return ;
    }

    p = fork();
    if (p < 0) {
        fprintf(stderr, "Fork Failed\n");
        return;
    } else if (p == 0) {
        // Child process
        close(fd[1]);  // Close the writing end of the pipe
        close(STDIN_FILENO);  // Close the current stdin
        dup2(fd[0], STDIN_FILENO);  // Replace stdin with the reading end of the pipe
        execl("bin/mail-out", "bin/mail-out", rec.c_str(), NULL);  // Invoke mail-out
    } else {
        // Parent process
        int status;
        close(fd[0]);  // Close the reading end of the pipe
        write(fd[1], buffer.c_str(), buffer.length() + 1);
        p = wait(&status);
        if (WEXITSTATUS(status) == 1)
            fprintf(stderr, "Err: Invalid Receiver %s\n", rec.c_str());
        else if (WEXITSTATUS(status) == 2)
            fprintf(stderr, "Err: System Func Err\n");
    }

}

int main() {

    split();

    return 0;
}