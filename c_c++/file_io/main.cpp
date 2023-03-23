// 文件IO：指不带缓冲的IO，属于POSIX.1和UNIX
// 常用函数：open、read、write、lseek、close
// 相关函数：dup、fcntl、sync、fsync、ioctl

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>           // 定义了文件IO的标准输入/输出/错误：STDIN_FILENO、STDOUT_FILENO、STDERR_FILENO(遵守POSIX.1规则)
#include <iostream>
#include <bits/stdc++.h>      // bitset头文件，用来十进制和二进制转换
using namespace std;

#define BUFFSIZE 15

const char path[] = "/home/mi/work/c_c++/file_io/test.txt";

// int open(const char *path, int flag, ... /* mode_t mode */ ); 成功返回文件描述符，失败返回-1
// open函数用来创建或打开一个文件
// path：文件路径；flag：文件选项；mode：当存在创建文件的flag时，需要指定文件的权限位
// 常用flag：O_RDONLY(只读打开); O_WRONLY(只写打开); O_RDWR(读写打开); O_APPEND(将写追加到文件末尾)； O_SYNC(每次 write都会等待直到内存中的修改同步到磁盘)
// 常用flag：O_CLOEXEC(设置文件描述符标志为FD_CLOEXEC); O_CREAT(文件不存在时创建文件); O_TRUNC(如果以只写或读写打开文件，将长度截断为0)； O_CLOEXEC(将FD_CLOEXEC设置为文件描述符标志)
int s_open() {
    int fd;
    if((fd = open(path, O_RDWR | O_CREAT | O_APPEND, 0777)) == -1) {
        cout << "open file failed" << endl;
    } else {
        cout << "open file succeed, fd = " << fd << endl;
    }
    return fd;
}

// int close(int fd); 成功返回0，失败返回-1
// 关闭一个打开的文件，并释放该进程加在该文件上的所有记录锁
void s_close(int fd) {
    if(close(fd) == -1) {
        cout << "close file failed" << endl;
    } else {
        cout << "close file succeed" << endl;
    }
}

// off_t lseek(int fd, off_t offset, int whence); 成功返回文件偏移量，失败返回-1
// 当打开一个文件时，除非设置了O_APPEND标志位，默认偏移量为0,lseek用来显式为文件设置偏移量
// offset与whence的值有关：
// 如果whence是 SEEK_SET，将文件的偏移量设置为距文件开始处offset字节
// 如果whence是 SEEK_CUR，将文件的偏移量设置为当前值加offset，offset可正可负
// 如果whence是 SEEK_END，将文件的偏移量设置为文件长度加offset，offset可正可负
void s_lseek() {
    int fd = s_open();

    off_t currpos;
    currpos = lseek(fd, 0, SEEK_CUR);      //查看标准输出当前的文件偏移量
    if(currpos == -1) {
        cout << "lseek failed" << endl;
    } else {
        cout << "current position of file is " << currpos << endl;
    }

    currpos = lseek(fd, 1, SEEK_CUR);        //将标准输出的文件偏移量+1
    if(currpos == -1) {
        cout << "lseek failed" << endl;
    } else {
        cout << "after lseek for file, current position is " << currpos << endl;
    }

    s_close(fd);
}

// ssize_t write(int fd, const void *buf, size_t nbytes); 成功返回已写的字节数，失败返回-1
// 向打开的文件写数据，默认从文件偏移量开始写，如果设置了O_APPEND标志位，则每次写操作之前都会把文件偏移量设置为当前结尾
void s_write() {
    int fd = s_open();

    ssize_t size;
    char buf[] = "write into file\n";

    if((size = write(fd, buf, sizeof(buf) - 1)) == -1) {        // sizeof获得的是字符数组所占用的内存地址，包括字符串结尾\0，所以这里必须-1，否则会乱码
        cout << "write failed" << endl;
    } else {
        cout << "write succeed" << endl;
    }

    s_close(fd);
}

// ssize_t read(int fd, void *buf, size_t nbytes); 成功返回读到的字节数，失败返回-1
// 从打开的文件读取数据。buf是用来存储的字符串；nbytes代表读取字符串长度
void s_read() {
    int fd = s_open();

    ssize_t size;
    char buf[BUFFSIZE];

    if((size = read(fd, buf, BUFFSIZE)) == -1) {
        cout << "read failed" << endl;
    } else {
        cout << "read buf size = " << size << " and buf is : " << buf << endl;
    }

    s_close(fd);
}

// int dup(int fd); int dup2(int fd, int fd2); 成功返回文件描述符，失败返回-1
// 用来复制一个现有的文件描述符
// dup复制的文件描述符是当前可用的文件描述符最小值
// dup2可以通过fd2指定新描述符的值，如果fd2已经打开了，则首先将其关闭
void s_dup() {
    int fd = s_open(), fd2, fd3;

    cout << "current fd = " << fd << endl;
    if ((fd2 = dup(fd)) == -1) {
        cout << "dup failed" << endl;
    } else {
        cout << "using dup to copy a fd, the new fd = " << fd2 << endl;
    }

    if ((fd3 = dup2(fd, 0)) == -1) {                // 这里首先关闭标准输入，然后将标准输入0重定向到fd
        cout << "dup failed" << endl;
    } else {
        cout << "using dup2 to copy a fd, the new fd3 = " << fd3 << endl;
    }

    ssize_t size;
    char buf[BUFFSIZE];

    if((size = read(fd3, buf, BUFFSIZE)) == -1) {       // 通过dup2复制后，标准输入和fd指向的文件表是相同的，即拥有相同的inode结点和偏移量
        cout << "read failed" << endl;
    } else {
        cout << "read buf size = " << size << " and buf read from fd 0 is : " << buf << endl;
    }

    s_close(fd);
}

// int sync(); int fsync(fd); 成功返回0，失败返回-1
// 将缓存在内存中的数据块写入磁盘
// sync将所有修改过的数据块排入写队列，并不等待写磁盘操作完成就会返回（通常内核会周期性的调用sync来定期flush内核缓冲区）
// fsync用于sync fd指定的文件，并且会等待写磁盘操作结束后才返回
void s_sync() {
    int fd = open(path, O_RDWR | O_CREAT | O_APPEND | O_SYNC, 0777);        // 添加O_SYNC标志位，每次 write都会将内存中的修改同步

    ssize_t size;
    char buf[] = "study sync\n";

    // 查看test.txt文件可知，采用每次write触发sync的情况下，数据是慢慢写完，花费时间很长，原因时sync时需要同步到磁盘，而磁盘的寻址速度远远低于内存的速度
    // 当不使用sync时，修改的数据块就会堆积到内存缓冲区，后续一次性同步到磁盘，速度很快
    for(int i = 0; i < 50; i++){
        write(fd, buf, sizeof(buf) - 1);    
    }

    cout << "sync completed " << endl;

    s_close(fd);
}

// int fcntl(int fd, int cmd, ... /* int arg */); 成功时返回值依赖参数 cmd，失败返回-1
// 改变已打开文件的属性
// cmd = F_DUPFD：复制一个已有的描述符，成功时返回文件描述符。同dup2(fd1，fd2),但不具备dup2的原子性（因为dup2分两步：关闭fd2和复制fd1）
// cmd=F_GETFD或F_SETFD：获取/设置文件描述符标志，GETFD成功返回文件描述符标志，默认为0；SETFD成功返回 0
// cmd=F_GETFL或F_SETFL：获取/设置文件状态标志，GETFL成功返回文件状态标志；SETFL成功返回 0
// cmd=F_GETOWN或F_SETOWN：获取/设置异步IO所有权，返回正的进程ID或负的进程组ID
// 文件描述符标志：目前只有一种文件描述符标志FD_CLOEXEC，一旦设置了该标志，会在进程执行exec时关闭该fd，避免了执行exec后进程某些fd无法关闭的问题
// 文件状态标志：调用open打开或创建文件时设置的flag
// 异步IO所有权：
void s_fcntl() {
    int fd = s_open();

    int fd_flag, fd_state;            // fd_flag代表文件描述符标志； fd_state代表文件状态标志

    if((fd_flag = fcntl(fd, F_GETFD)) == -1){
        cout << "F_GETFD failed" << endl;
    } else {
        cout << "fd flag = " << fd_flag << endl;
    }

    fd_flag |= FD_CLOEXEC;              // 添加FD_CLOEXEC文件描述符标志

    if(fcntl(fd, F_SETFD, fd_flag) == -1){
        cout << "F_SETFD failed" << endl;
    } else {
        cout << "add FD_CLOEXEC to fd flag = " << fcntl(fd, F_GETFD) << endl;
    }

    if((fd_state = fcntl(fd, F_GETFL)) == -1){
        cout << "F_GETFL failed" << endl;
    } else {
        bitset<20> bit(fd_state);             // 转换为20位二进制
        cout << "fd state = " << fd_state << "  and bit = " << bit << endl;
    }
    
    fd_state |= O_NONBLOCK;         // 添加O_NONBLOCK文件状态标志


    if(fcntl(fd, F_SETFL, fd_state) == -1){
        cout << "F_SETFL failed" << endl;
    } else {
        bitset<20> bit(fcntl(fd, F_GETFL));
        cout << "add O_NONBLOCK to fd state = " << fcntl(fd, F_GETFL) << "  and bit = " << bit << endl;
    }

    s_close(fd);
}


// int ioctl(int fd, int request, ... ); 成功返回驱动程序中设置好的值，失败返回-1
// 用于在用户空间执行一些设备驱动程序中设置好的命令
void s_ioctl() {
    // fd = open("/dev/memdev0",O_RDWR);            // 获取设备的fd
    // cmd = MEMDEV_IOCPRINT;                       // 对设备执行的命令，这个命令在设备驱动中通过switch case设置，需要在驱动程序中找到
    // ioctl(fd, cmd, &arg)；                       // 调用该命令在驱动程序中对应的功能
}

int main(void) {
    open(path, O_RDWR | O_TRUNC | O_CREAT, 0777);     // 将文件截断为0，方便调试

    int fd = s_open(); // open()的用法

    s_close(fd);       // close()的用法

    s_lseek();         // lseek()的用法

    s_write();         // write()的用法

    s_read();          // read()的用法

    s_dup();           // dup()/dup2()的用法

    s_sync();          // sync()的用法

    s_fcntl();         // fcntl()的用法

    s_ioctl();         // ioctl()的用法

    return 0;
}