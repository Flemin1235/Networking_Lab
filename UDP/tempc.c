#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>


FILE *fp;
char fname[100];

int main(){
    int soc, cltsoc, port = 8888;
    struct sockaddr_in servadd, cltadd;
    socklen_t len = sizeof(servadd);
    int num;
    char msg[100];

    soc = socket(AF_INET, SOCK_DGRAM, 0);

    servadd.sin_family = AF_INET;
    servadd.sin_port = htons(port);
    servadd.sin_addr.s_addr = inet_addr("10.0.1.137");


    printf("\nEnter a number: ");
    scanf("%d", &num);

    sendto(soc, &num, sizeof(num), 0, (struct sockaddr *)&servadd, len);
    printf("\nNumber sent to server!!");

    recvfrom(soc, msg, sizeof(msg), 0, (struct sockaddr *)&servadd, &len);
    printf("\nResult from server: %s", msg);

    close(soc);

}