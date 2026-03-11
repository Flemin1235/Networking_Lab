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

	int servsoc, port;
	struct sockaddr_in servadd, cltadd;
	socklen_t len = sizeof(cltadd);
	char msg[100];

	servsoc = socket(AF_INET, SOCK_DGRAM, 0);
	
	printf("\nEnter port no: ");
	scanf("%d", &port);

	servadd.sin_family  = AF_INET;
	servadd.sin_port = htons(port);
	servadd.sin_addr.s_addr = INADDR_ANY;

	bind(servsoc, (struct sockaddr *)&servadd, sizeof(servadd));
	

	int expectedseq = 0;

	Frame recvfr, sendfr;

	while(1){
		recvfrom(servsoc, &recvfr, sizeof(Frame), 0, (struct sockaddr *)&cltadd, &len);

		if(recvfr.kind == 1 && recvfr.seq_no == expectedseq){
			printf("\nFrame recieved with Seq No: %d", recvfr.seq_no);
			printf("\nFrame: %s", recvfr.pkt.data);

			sendfr.kind = 0;
			sendfr.ack = recvfr.seq_no;
			
			printf("\nACK sent !!");

			sendto(servsoc, &sendfr, sizeof(Frame), 0, (struct sockaddr *)&cltadd, len);

			
			expectedseq = 1 - expectedseq;
		}
		else{
			printf("\nDuplicate Frame !! Discarding ....");
			sendfr.kind = 0;
			sendfr.ack = expectedseq;

			sendto(servsoc, &sendfr, sizeof(Frame), 0, (struct sockaddr *)&cltadd, len);
		}
	}

	close(servsoc);
}