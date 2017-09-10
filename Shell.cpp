#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>

#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>
#include <curses.h>

using namespace std;

string getcwd_string(void) {
    char buff[PATH_MAX];
    getcwd( buff, PATH_MAX );
    std::string cwd( buff );
    return cwd;
}

int main(void) {
    DIR *d;
    struct dirent *de;
    int i, c, j, k;
    string in;
    bool flag = false;
    while(!flag) {
        auto t = time(nullptr);
        auto time_t = *localtime(&t);
        cout << "Time: " << put_time(&time_t, "%d-%m-%Y    %H:%M:%S") << endl;
        cout << "\nCurrent Directory: " << getcwd_string() << endl;
        d = opendir(".");
        int c = 0, j = 0;
        /* Use one of the above counting variables and 2 loops for directories
        and files, adding each to a string vector with the type concat to the
        front of the string like vector.push("Directory: "+de->d_name);
        Then use the total number of items in the vector and print x amount
        at a time if greater than a certain number
        */
        while (de = readdir(d)) {
            /*while c*/
        if ((de->d_type) & DT_DIR)
            printf( "( %d Directory:  %s )\n", c++, de->d_name);
        }
        cin >> in;
        if(in == "q") {
            flag = true;
            break;
        }
    }
    return 0;
}
