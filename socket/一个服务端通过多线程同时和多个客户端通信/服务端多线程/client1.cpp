#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#define MAXLINE 4096

int main(int argc, char** argv){
    int   sockfd, n;
    char  recvline[4096], sendline[] = "hellozn_in_client1", recvice_data[400];
    struct sockaddr_in  servaddr;

    if( argc != 2){
        printf("usage: ./client <ipaddress>\n");
        return 0;
    }

    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666);
    if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
        printf("inet_pton error for %s\n",argv[1]);
        return 0;
    }

    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    printf("send msg to server: \n");
    if( send(sockfd, sendline, strlen(sendline), 0) < 0){
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }
    printf("send msg %s to server succeed \n", sendline);

    memset(recvice_data,'\0',strlen(recvice_data));
    if(recv(sockfd, recvice_data, 400, 0) < 0 ) {
        printf("recv msg error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }
    printf("recvice msg %s from server succeed \n", recvice_data);
    printf("sleep 5s......\n");
    sleep(5);
    close(sockfd);
    return 0;
}