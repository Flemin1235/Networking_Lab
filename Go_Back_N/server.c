#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#define MAX_SEQ 8

typedef struct{
    int seq_no;
    char data[1024];
} Frame;

typedef struct{
    int ack;
} Ack;

int main(){

    int servsock, port;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);

    Frame frame;
    Ack ack;

    int expected = 0;

    servsock = socket(AF_INET, SOCK_DGRAM, 0);

    printf("Enter port: ");
    scanf("%d",&port);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    bind(servsock,(struct sockaddr*)&servaddr,sizeof(servaddr));

    while(1){

        recvfrom(servsock,&frame,sizeof(frame),0,
                (struct sockaddr*)&cliaddr,&len);

        printf("\nFrame received: %d",frame.seq_no);

        if(frame.seq_no == expected){

            printf("\nAccepted: %s",frame.data);

            ack.ack = frame.seq_no;

            sendto(servsock,&ack,sizeof(ack),0,
                   (struct sockaddr*)&cliaddr,len);

            printf("\nACK sent: %d\n",ack.ack);

            expected = (expected + 1) % MAX_SEQ;
        }
        else{

            printf("\nOut of order frame discarded");

            ack.ack = (expected - 1 + MAX_SEQ) % MAX_SEQ;

            sendto(servsock,&ack,sizeof(ack),0,
                   (struct sockaddr*)&cliaddr,len);

            printf("\nResent ACK: %d\n",ack.ack);
        }
    }

    close(servsock);
}