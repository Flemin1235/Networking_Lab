#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>




int main(){
    int soc, cltsoc, port = 8888;
    struct sockaddr_in servadd, cltadd;
    socklen_t len = sizeof(servadd);
    int num;
    char msg[100];

    soc = socket(AF_INET, SOCK_STREAM, 0);

    servadd.sin_family = AF_INET;
    servadd.sin_port = htons(port);
    servadd.sin_addr.s_addr = inet_addr("10.0.1.137");


    if(connect(soc, (struct sockaddr *)&servadd, len) != -1){
        printf("\nConnected!!");
    };



    printf("\nEnter a number: ");
    scanf("%d", &num);

    send(soc, &num, sizeof(num), 0);
    printf("\nMessage sent to server!!");

    recv(soc, msg, sizeof(msg), 0);
    printf("\nMessage from server: %s", msg);

    close(soc);

}