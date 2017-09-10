/**
* Name: Nicholas Reimherr
* ID: 1001179884
* Language: C++
* Assignment 1: Simple Shell
*/

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <cstdlib>

#include <curses.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

using namespace std;

/**
* Function to return the current working directory as a string
* Note: Uses PATH_MAX in order to avoid using an arbitrary fixed length buffer.
*/
string getcwd_string(void) {
    char buffer[PATH_MAX];
    getcwd(buffer, PATH_MAX);
    string dir(buffer);
    return dir;
}

int main(void) {
    /* used when forking the process to edit a file or run a program */
    pid_t state;
    DIR *d;
    struct dirent *de;
    int i, c, k;
    /* Vector to store each item found in a directory for easy navigation */
    vector<string> list;
    string in;
    bool flag = false;
    while(!flag) {
        auto t = time(nullptr);
        auto time_t = *localtime(&t);
        cout << "Time: " << put_time(&time_t, "%d-%m-%Y    %H:%M:%S") << endl;
        cout << "\nCurrent Directory: " << getcwd_string() << endl;
        d = opendir(".");
        int c = 0, j = 0;
        while (de = readdir(d)) {
            if ((de->d_type) & DT_DIR) {
                string temp = "( " + to_string(c) + " Directory: " + de->d_name + " )";
                list.push_back(temp);
                c++;
            }
        }
        closedir(d);
        d = opendir(".");
        c = 0;
        while (de = readdir(d)) {
            if((de->d_type) & DT_REG) {
                string temp = "( " + to_string(c) + " File: " + de->d_name + " )";
                list.push_back(temp);
                c++;
            }
        }
        /* Variables to keep track of the current items printed to the screen
         eg 0-16, 16-32, etc. Added the ability to go back and forth.*/
        int lim = 16;
        c = 0;
        string nxt;
        if(list.size() > lim) {
            while(c < lim && c < list.size()) {
                cout << list[c++] << endl;
            }
            cout << "Enter n for Next, or x to return to commands." << endl;
            while(cin >> nxt) {
                if(nxt == "n") {
                    system("clear");
                    lim = lim+16;
                    while(c < lim && c < list.size()) {
                        cout << list[c++] << endl;
                    }
                } if(nxt == "p" && (c-16) >= 0) {
                    system("clear");
                    lim = lim-16;
                    c = c-32;
                    cout << c << "\t" << lim << endl;
                    while(c < lim && c < list.size()) {
                        cout << list[c++] << endl;
                    }
                } else if(nxt == "x") {
                    break;
                }
                if(c >= list.size()) {
                    break;
                }
                cout << "Enter n for Next, p for prev, or x to return to commands." << endl;
            }
        } else {
            while(c < list.size()) {
                cout << list[c++] << endl;
            }
        }
        cout << "--------------------------------------------" << endl;
        cout << "[c to change directory, r to run, e to edit]" << endl;
        cin >> in;
        if(in != "c" && in != "r" && in != "e" && in != "s" && in != "q") {
            cout << "Invalid command." << endl;
        }
        if(in == "q") {
            flag = true;
            break;
        }
        if(in == "c") {
            string cmd;
            cout << "Change To?: ";
            cin >> cmd;
            if(access(cmd.c_str(), F_OK|R_OK) == 0) {
                cout << cmd << endl;
                chdir(cmd.c_str());
            } else {
                cout << "Directory does not exist." << endl;
                chdir( "." );
            }
        }
        if(in == "r") {
            cout << "Run What?: ";
            string cmd;
            cin >> cmd;
            /* This will check if the file exists and it can be executed to avoid
            any system call errors. */
            if(access(cmd.c_str(), F_OK|X_OK) == 0) {
                state = fork();
                if(state == 0) {
                    system("clear");
                    execl(cmd.c_str(), (char*) 0);
                    exit(1);
                }
                wait(&state);
            } else {
                cout << "\nEither the file doesn't exist or it is not executable\n" << endl;
            }
        }
        if(in == "e") {
            string file_dir;
            cout << "Edit What?: ";
            string cmd;
            cin >> cmd;
            /* Check to ensure the file exists and the user has read and
            write access */
            if(access(cmd.c_str(), F_OK|R_OK|W_OK) == 0) {
                state = fork();
                file_dir = "-o "+getcwd_string();
                /*cout << "state: " << state << "\tfile: "<< file_dir <<endl;*/
                if(state == 0) {
                    /* need the -o command to specify working directory so that
                    an existing file will be opened rather than a new buffer */
                    execlp("pico", file_dir.c_str(), cmd.c_str(), (char *) 0);
                    exit(1);
                }
                wait(&state);
            }
        }
        if(in == "s") {
            cout << "Press d to sort by date, z to sort by size" << endl;
        }
        list.clear();
    }
    return 0;
}
