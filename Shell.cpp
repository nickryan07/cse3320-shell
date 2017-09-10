#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <cstdlib>

#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <curses.h>

using namespace std;

string getcwd_string(void) {
    char buffer[PATH_MAX];
    getcwd(buffer, PATH_MAX);
    string dir(buffer);
    return dir;
}

int main(void) {
    pid_t state;
    DIR *d;
    struct dirent *de;
    int i, c, k;
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
        /*TODO : Implement prev feature to go back and forth in the vector
        display. */
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
            state = fork();
            if(state == 0) {
                execl(cmd.c_str(), (char*) 0);
                exit(1);
            }
            wait(&state);
        }
        if(in == "e") {
            string file_dir;
            cout << "Edit What?: ";
            string cmd;
            cin >> cmd;
            state = fork();
            file_dir = "-o "+getcwd_string();
            /*cout << "state: " << state << "\tfile: "<< file_dir <<endl;*/
            if(state == 0) {
                execlp("pico", file_dir.c_str(), cmd.c_str(), (char *) 0);
                exit(1);
            }
            wait(&state);
        }
        if(in == "s") {
            cout << "Press d to sort by date, z to sort by size" << endl;
        }
        list.clear();
    }
    return 0;
}
