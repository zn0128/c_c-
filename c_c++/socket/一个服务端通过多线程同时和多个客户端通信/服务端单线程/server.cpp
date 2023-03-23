#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <pthread.h>
#include<unistd.h>

#define MAXLINE 4096
int clientSocket[1024];

typedef struct __DATA
{
    int     i;
    char *buff;
    char *send_data;
}DATA;

void *recvice_and_send_msg(void* param) { //int i, char *buff, char *send_data
    DATA* data = (DATA*)param;
    int i = data->i;
    char *buff = data->buff;
    char *send_data = data->send_data;
    while(true) {
        int n = recv(clientSocket[i], buff, 400, 0);
        if (n < 0) {
            printf("recv msg from client failure: \n");
        }
        printf("recv msg from client: %s\n", buff);
        n = send(clientSocket[i], send_data, strlen(send_data), 0);
        if (n < 0) {
            printf("send msg to client failure: \n");
        }
        printf("send msg to client: %s\n", send_data);
    }
}

int main(int argc, char** argv){
    int  listenfd, ret, count = 0;
    struct sockaddr_in  servaddr;
    char  buff[4096], send_data[] = "helloworld";
    int  n;
    pthread_t tid[1024];
    DATA data;

    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6666);

    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    if( listen(listenfd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    printf("======waiting for client's request: ======\n");

    while(1){
        if( (clientSocket[count] = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            return 0;
        }
        data = {0};
        data.i = count;
        data.buff = buff;
        data.send_data = send_data;

        recvice_and_send_msg(&data);
        count++;
    }

    for (int i = 0; i < count; i++) {
        close(clientSocket[i]);
    }
    close(listenfd);
    return 0;
}