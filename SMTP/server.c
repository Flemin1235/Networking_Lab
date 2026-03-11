#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define MAX 100

int main()
{
    int sock,n;
    struct sockaddr_in server,client;
    socklen_t len;
    char buffer[MAX];



    sock=socket(AF_INET,SOCK_DGRAM,0);

    printf("\nEnter port: ");
    int port;
    scanf("%d", &port);
    getchar();

    server.sin_family=AF_INET;
    server.sin_port=htons(port);
    server.sin_addr.s_addr=INADDR_ANY;

    bind(sock,(struct sockaddr*)&server,sizeof(server));

    len=sizeof(client);

    /* Receive request */
    n=recvfrom(sock,buffer,MAX,0,(struct sockaddr*)&client,&len);
    buffer[n]='\0';
    printf("Client: %s",buffer);

    strcpy(buffer,"220 SMTP Server Ready\n");
    sendto(sock,buffer,strlen(buffer),0,(struct sockaddr*)&client,len);

    /* HELLO */
    n=recvfrom(sock,buffer,MAX,0,(struct sockaddr*)&client,&len);
    buffer[n]='\0';
    printf("Client: %s",buffer);

    strcpy(buffer,"250 Hello Client\n");
    sendto(sock,buffer,strlen(buffer),0,(struct sockaddr*)&client,len);

    /* MAIL FROM */
    n=recvfrom(sock,buffer,MAX,0,(struct sockaddr*)&client,&len);
    buffer[n]='\0';
    printf("Client: %s",buffer);

    strcpy(buffer,"250 Sender OK\n");
    sendto(sock,buffer,strlen(buffer),0,(struct sockaddr*)&client,len);

    /* RCPT TO */
    n=recvfrom(sock,buffer,MAX,0,(struct sockaddr*)&client,&len);
    buffer[n]='\0';
    printf("Client: %s",buffer);

    strcpy(buffer,"250 Receiver OK\n");
    sendto(sock,buffer,strlen(buffer),0,(struct sockaddr*)&client,len);

    /* DATA */
    n=recvfrom(sock,"DATA",MAX,0,(struct sockaddr*)&client,&len);
    buffer[n]='\0';
    printf("Client: %s",buffer);

    strcpy(buffer,"354 Enter message, end with .\n");
    sendto(sock,buffer,strlen(buffer),0,(struct sockaddr*)&client,len);

    while(1){
        n=recvfrom(sock,buffer,MAX,0,(struct sockaddr*)&client,&len);
        buffer[n]='\0';

        if(strcmp(buffer,".\n")==0)
            break;

        printf("Message: %s",buffer);
    }

    strcpy(buffer,"221 Connection closed\n");
    sendto(sock,buffer,strlen(buffer),0,(struct sockaddr*)&client,len);

    return 0;
}