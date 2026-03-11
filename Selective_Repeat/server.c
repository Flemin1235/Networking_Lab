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
    struct sockaddr_in servadd, cltadd;
    socklen_t len = sizeof(cltadd);
    char buffer[100];

    cltsoc = socket(AF_INET, SOCK_DGRAM, 0);

    servadd.sin_family = AF_INET;
    servadd.sin_port = htons(port);
    servadd.sin_addr.s_addr = INADDR_ANY;

    bind(cltsoc, (struct sockaddr *)&servadd, sizeof(servadd));

    int expected = 0;

    Frame frame;
    ACK ack;


    while(1){
        int n = recvfrom(cltsoc, &frame, sizeof(Frame), 0, (struct sockaddr *)&cltadd, &len);

        if(frame.seq_no >= expected, frame.seq_no < WINDOW + expected){
            printf("\nFrame SEQ NO %d recieved: %s", frame.seq_no, frame.data);

            ack.ack_no = frame.seq_no;

            sendto(cltsoc, &ack, sizeof(ACK), 0, (struct sockaddr *)&cltadd, len);
            printf("\nACK sent for SEQ NO %d", expected);

            expected = (expected + 1) % MAXSEQ;
        }

        else{
            printf("\nOut of order frame recieved!!");

            ack.ack_no = (expected - 1 + MAXSEQ) % MAXSEQ;

            sendto(cltsoc, &ack, sizeof(Frame), 0, (struct sockaddr *)&cltadd, len);
        }
    }

    close(cltsoc);


}