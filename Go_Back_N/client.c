#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include<unistd.h>

#define WINDOW_SIZE 4
#define MAX_SEQ 8

typedef struct{
    int seq_no;
    char data[1024];
} Frame;

typedef struct{
    int ack;
} Ack;

int main(){

    int sock, port;
    struct sockaddr_in servaddr;
    socklen_t len = sizeof(servaddr);

    Frame frame[MAX_SEQ];
    Ack ack;

    int base = 0;
    int nextseq = 0;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    printf("Enter port: ");
    scanf("%d",&port);
    getchar();

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr("10.1.4.202");

    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));

    while(1){

        while(nextseq < base + WINDOW_SIZE){

            printf("\nEnter message: ");
            fgets(frame[nextseq % MAX_SEQ].data,1024,stdin);

            frame[nextseq % MAX_SEQ].seq_no = nextseq % MAX_SEQ;

            sendto(sock,&frame[nextseq % MAX_SEQ],
                   sizeof(Frame),0,
                   (struct sockaddr*)&servaddr,len);

            printf("Frame %d sent\n",
                   frame[nextseq % MAX_SEQ].seq_no);

            nextseq++;
        }

        int n = recvfrom(sock,&ack,sizeof(ack),0,
                        (struct sockaddr*)&servaddr,&len);

        if(n > 0){

            printf("ACK received: %d\n",ack.ack);

            base = (ack.ack + 1) % MAX_SEQ;
        }
        else{

            printf("Timeout... Resending window\n");

            for(int i=base;i<nextseq;i++){

                sendto(sock,&frame[i % MAX_SEQ],
                       sizeof(Frame),0,
                       (struct sockaddr*)&servaddr,len);

                printf("Resent frame %d\n",
                       frame[i % MAX_SEQ].seq_no);
            }
        }
    }

    close(sock);
}