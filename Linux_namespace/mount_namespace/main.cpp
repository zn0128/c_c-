#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>

#define STACK_SIZE (1024*1024)

static char child_stack[STACK_SIZE];

int child_fn1(void *args) {
    printf("\n子线程1, pid = %d\n", getpid());
    mount("proc", "/home/mi/work/c++/Linux_namespace/mount_namespace/zn", "proc", 0, NULL);
    sethostname("zn_host", 12);
    printf("子线程1的hostname：\n");
    system("hostname\n");
    printf("子线程1的namespace fd：\n");
    system("ls -l /proc/self/ns/mnt\n");
    return 1;
}

int child_fn2(void *args) {
    printf("\n子线程2, pid = %d\n", getpid());
    printf("setns修改namespace fd前：\n");
    system("ls -l /proc/self/ns/mnt\n");
    setns(4545, 0);                             //这里还有问题
    printf("setns修改namespace fd后：\n");
    system("ls -l /proc/self/ns/mnt\n");
    return 1;
}

int child_fn3(void *args) {
    printf("\n子线程3, pid = %d\n", getpid());
    printf("unshare修改namespace fd前：\n");
    system("ls -l /proc/self/ns/mnt\n");
    unshare(CLONE_NEWNS | CLONE_FILES | CLONE_FS);
    printf("unshare修改namespace fd后：\n");
    system("ls -l /proc/self/ns/mnt\n");
    return 1;
}

int main(int argc, char** argv){
    printf("主线程, pid = %d\n", getpid());
    printf("主线程的hostname：\n");
    system("hostname\n");
    printf("主线程的namespace fd：\n");
    system("ls -l /proc/self/ns/mnt\n");

    pid_t child_pid1 = clone(child_fn1, child_stack + STACK_SIZE, SIGCHLD | CLONE_NEWNS | CLONE_NEWUTS , NULL);
    waitpid(child_pid1, NULL, 0);

    pid_t child_pid2 = clone(child_fn2, child_stack + STACK_SIZE, SIGCHLD, NULL);
    waitpid(child_pid2, NULL, 0);

    pid_t child_pid3 = clone(child_fn3, child_stack + STACK_SIZE, SIGCHLD, NULL);
    waitpid(child_pid3, NULL, 0);
    return 0;
}