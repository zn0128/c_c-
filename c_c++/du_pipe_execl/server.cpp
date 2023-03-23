#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <sys/wait.h>
using namespace std;

// template<class T, T nullvalue, class D, D d> // auto d works in new compilers.
// struct generic_delete
// {
//     class pointer
//     {
//         T t;
//     public:
//         pointer(T t) : t(t) {}
//         pointer(std::nullptr_t = nullptr) : t(nullvalue) { }
//         explicit operator bool() { return t != nullvalue; }
//         friend bool operator ==(pointer lhs, pointer rhs) { return lhs.t == rhs.t; }
//         friend bool operator !=(pointer lhs, pointer rhs) { return lhs.t != rhs.t; }
//         operator T() { return t; }
//     };
//     void operator()(pointer p)
//     {
//         d(p);
//     }
// };


// using unique_fd = std::unique_ptr<void, generic_delete<int, -1, decltype(&close), &close>>;      // Android中有现成的
char dump_path[] = "/home/mi/work/c_c++/z.txt";

void RunDuCommand(int fd) {
    const char kDuPath[] = "/usr/bin/du";       // Android源码中换成/system/bin/du
    const char kSortPath[] = "/usr/bin/sort";       // Android源码中换成/system/bin/sort
    int stat_val;

    // int fds[2];
    // if (pipe(fds) != 0) {
    //     printf("Failed to create pipe");
    // }
    // unique_fd read_fd(fds[0]);
    // unique_fd write_fd(fds[1]);
    //fcntl(read_fd.get(), F_SETFL, O_CLOEXEC | O_NONBLOCK);

    const pid_t child_pid1 = fork();
    if (child_pid1 == -1) {
        printf("Failed to fork child");
    }
    if (child_pid1 == 0) {  // We are in the child process.
        close(0);  // Don't want to read anything in this process.
        //dup2(fd, 1);  // Replace existing stdout with the pipe.
        // read_fd.reset();
        // write_fd.reset();
        system("du -d 5 /home/mi/work/c_c++/ | sort -n -k1 > /home/mi/work/c_c++/z.txt");

        // if (execl(kDuPath, "du", "-d 10", "/home/mi/work/c_c++/", nullptr) < 0) {
        //     cout << "exec error" << endl;
        // }
    }
    waitpid(child_pid1, &stat_val, 0);
    if (WIFEXITED(stat_val)) {
        cout << "child_pid1 exited with code " << WEXITSTATUS(stat_val) << endl;
    } else {
        cout << "child_pid1 exited abnormally" << endl;
    }


    // const pid_t child_pid2 = fork();
    // if (child_pid2 == -1) {
    //     printf("Failed to fork child");
    // }
    // if (child_pid2 == 0) {  // We are in the child process.
    //     close(0);  // Don't want to read anything in this process.
    //     dup2(fd, 1);  // Replace existing stdout with the pipe.
    //     // read_fd.reset();
    //     // write_fd.reset();

    //     if (execl(kSortPath, "sort", "-n", "-k1", "/home/mi/work/c_c++/z.txt", nullptr) < 0) {
    //         cout << "exec error" << endl;
    //     }
    // }
    // waitpid(child_pid2, &stat_val, 0);
    // if (WIFEXITED(stat_val)) {
    //     cout << "child_pid2 exited with code " << WEXITSTATUS(stat_val) << endl;
    // } else {
    //     cout << "child_pid2 exited abnormally" << endl;
    // }
}

int main(int argc, char** argv){
    int fd = open(dump_path, O_RDWR | O_CREAT | O_TRUNC, 0777);
    RunDuCommand(fd);
    close(fd);
    return 0;
}