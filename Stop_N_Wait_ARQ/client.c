#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include<unistd.h>

typedef struct{
    char data[1024];
} Packet;

typedef struct{
    int kind; // 0 ACK and 1 DATA
    int seq_no, ack;
    Packet pkt;
} Frame;

int main(){

    int cltsoc, port;
    struct sockaddr_in servadd;
    socklen_t len = sizeof(servadd);
    char buffer[1024];

    Frame sendfr, recvfr;

    cltsoc = socket(AF_INET, SOCK_DGRAM, 0);

    printf("\nEnter port no: ");
    scanf("%d", &port);
    getchar();   // clear newline

    servadd.sin_family  = AF_INET;
    servadd.sin_port = htons(port);
    servadd.sin_addr.s_addr = inet_addr("10.0.1.202");

    int seq_no = 0;

    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    setsockopt(cltsoc, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while(1){

        printf("\nEnter a message: ");
        fgets(buffer,1024,stdin);

        strcpy(sendfr.pkt.data, buffer);

        sendfr.kind = 1;
        sendfr.seq_no = seq_no;
        sendfr.ack = 0;

		SEND:

				sendto(cltsoc, &sendfr, sizeof(Frame), 0,
					(struct sockaddr *)&servadd, len);

				printf("\nFrame with Seq No %d sent!!", sendfr.seq_no);

				int n = recvfrom(cltsoc, &recvfr, sizeof(Frame), 0,
								(struct sockaddr *)&servadd, &len);

		if(n < 0){
			printf("\nTimeout.... Resending Frame!!");
			goto SEND;
		}

		if(recvfr.kind == 0 && recvfr.ack == seq_no){
			printf("\nACK received: %d", recvfr.ack);
			seq_no = 1 - seq_no;
		}
    }

    close(cltsoc);
}