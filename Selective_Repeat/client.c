#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/time.h>

#define WINDOW 4
#define MAXSEQ 8

typedef struct{
    int seq_no;
    char data[1024];
} Frame;

typedef struct{
    int ack_no;
} ACK;

int main(){

    int cltsoc, port = 8000;
    struct sockaddr_in servadd;
    socklen_t len = sizeof(servadd);
    char buffer[100];

    cltsoc = socket(AF_INET, SOCK_DGRAM, 0);

    servadd.sin_family = AF_INET;
    servadd.sin_port = htons(port);
    servadd.sin_addr.s_addr = inet_addr("10.0.3.156");

    int base = 0, nextseq = 0;

    Frame frame[MAXSEQ];
    ACK ack;

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    
    setsockopt(cltsoc, SOL_SOCKET, SO_RCVTIMEO, &tv ,sizeof(tv));

    while(1){
        while((base + WINDOW) > nextseq){

            printf("\nEnter a data: ");
            fgets(frame[nextseq % MAXSEQ].data, 1024, stdin);

            frame[nextseq % MAXSEQ].seq_no = nextseq % MAXSEQ;

            sendto(cltsoc, &frame[nextseq % MAXSEQ], sizeof(Frame), 0, (struct sockaddr *)&servadd, len);
            printf("\nFrame sent with SEQ NO %d", nextseq % MAXSEQ);

            nextseq++;
        }

        int n = recvfrom(cltsoc, &ack, sizeof(ACK), 0, (struct sockaddr *)&servadd, &len);
        if(n < 0){
            printf("\nTimeout occured...");
            sendto(cltsoc, &frame[base % MAXSEQ], sizeof(Frame), 0, (struct sockaddr *)&servadd, len);
            printf("\nResent Frame SEQ NO %d", base % MAXSEQ);
        }
        else{
            printf("\nACK recieved for SEQ NO %d", ack.ack_no);
            base = (ack.ack_no + 1) % MAXSEQ;
        }
    }

    close(cltsoc);


}