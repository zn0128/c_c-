#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;

int main(int argc, char* argv[]) { 
    // int output(dup(STDOUT_FILENO));
    int input(dup(STDIN_FILENO));
    char buf[100];

    pid_t pid = fork();
    if(pid == -1) {
        cout << "error" << endl;
        return 0;
    } else if (pid > 0) {
        //父进程
        // cout << "child argv msg = " << argv[0] << "  and  " << argv[1] << endl;
        // read(input, buf, sizeof(buf)); 
        // dup2(input, 0);
        // write(STDOUT_FILENO, buf, strlen(buf) + 1);
        sleep(3);
        cout << "father rev msg = " << endl;
        return 0;
    } else {
        //子进程
        // close(input);
        // write(output, argv[1], strlen(argv[1]) + 1);
    }

    read(input, buf, sizeof(buf)); 
    cout << "child rev msg = " << buf << endl;

    return 0;
}