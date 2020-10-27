#include <regex>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>

using namespace std;

string whitespaces(" \t\f\v\n\r");

bool validate(string);

void split() {
    string buf;
    int step = 0;
    string sender;
    string rcpt_line;
    vector<string> receivers;
    string message;
    ofstream tmp;
    while (getline (cin, buf)) {
        if (buf.compare(".") == 0) {
            if (step == 1) {
                fprintf(stderr, "Err: No RCPT TO\n");
            } else if (step == 0) {
                fprintf(stderr, "Err: No MAIL FROM\n");
            }
            for(int i=0; i<receivers.size(); i++) {
                invoke(rec, message);
            }
            message.clear();
            receivers.clear();
            step = 0;
            tmp.close();
            continue;
        }
        if (step == -1) {
            if (tmp.is_open()) {
            tmp.close();
        }
            continue;
        }
        if (buf.compare(0, 1, ".") == 0) {
            buf.erase(0,1);
        }
        if (!tmp.is_open()) {
            tmp.open("../tmp/tmp.txt", fstream::out);
        }
        if (step == 0) {
            if (buf.compare(0, 10, "MAIL FROM:") != 0) {
                fprintf(stderr, "Err: No MAIL FROM\n");
                cout << EOF;
                step = -1;
            } else if (buf.compare(10, 1, "<") != 0 || buf.find_last_of(">") != buf.find_last_not_of(whitespaces)){
                fprintf(stderr, "Err: Incorrect angle brackets\n");
                step = -1;
            } else {
                string username = buf.substr(11, buf.find_last_of(">")-11);
                bool nameOK = validate(username);
                if (!nameOK) {
                    fprintf(stderr, "Err: Bad Sender Username %s\n", username);
                    step = -1;
                } else {
                    sender = "From: " + username;
                    step++;
                }
            }
        } else if (step == 1) {
            if (buf.compare(0, 9, "RCPT TO:") != 0 && receivers.size() == 0) {
                fprintf(stderr, "Err: No RCPT TO:");
                step = -1;
            } else if (buf.compare(0, 9, "RCPT TO:") == 0) {
                if (buf.compare(9, 1, "<") != 0 || buf.find_last_of(">") != buf.find_last_not_of(whitespaces)){
                    fprintf(stderr, "Err: Incorrect angle brackets\n");
                } 
                string username = buf.substr(9, buf.find_last_of(">")-9);
                bool nameOK = validate(username);
                if (!nameOK) {
                    fprintf(stderr, "Err: Bad Sender Username %s\n", username);
                } else {
                    receivers.push_back(username);
                }   
            } else if (buf.compare("DATA") == 0) {
                rcpt_line = "    To: ";
                for (int i=0; i<receivers.size()-1; i++)
                    rcpt_line += receivers[i] + ", ";
                rcpt_line = receivers[receivers.size()-1];
                message = sender + "\n" + rcpt_line + "\n" + "\n";
                sender.clear()
                rcpt_line.clear()
                step++;
            }
        } else if (step == 2) {
            message += buf + "\n"
        }
    }

}

bool validate(string username) {
    ifstream MailboxListFile("mailbox_list.txt");
    string name;
    while (getline (MailboxListFile, name)) {
        if (username.compare(name)) {
            return 1;
        }
    }
    return 0;
}


void invoke(string rec, string buffer) {
    int fd[2];
    int a = 2, b = 3;
    pid_t p;

    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe Failed\n");
        return 1;
    }

    p = fork();
    if (p < 0) {
        fprintf(stderr, "Fork Failed\n");
        return 1;
    } else if (p == 0) {
        // Child process
        close(fd[1]);  // Close the writing end of the pipe
        close(STDIN_FILENO);  // Close the current stdin
        dup2(fd[0], STDIN_FILENO);  // Replace stdin with the reading end of the pipe
        execl("./mail-out", "./mail-out", rec, NULL);  // Invoke mail-out
    } else {
        // Parent process
        int status;
        close(fd[0]);  // Close the reading end of the pipe
        write(fd[1], buffer.c_str, buffer.length() + 1);
        p = wait(&status);
        if (WEXITSTATUS(status) != 0)
            fprintf(stderr, "Err: invalid receiver %s", rec);
    }

}

int main() {

    split();

    return 0;
}