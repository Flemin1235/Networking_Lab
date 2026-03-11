#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define MAX 100

int main()
{
    int sock,n;
    struct sockaddr_in server;
    char buffer[MAX];
    char sender[MAX],receiver[MAX],msg[MAX];


    sock=socket(AF_INET,SOCK_DGRAM,0);

    printf("\nEnter port: ");
    int port;
    scanf("%d", &port);
    getchar(); // To consume the newline character

    server.sin_family=AF_INET;
    server.sin_port=htons(port);
    server.sin_addr.s_addr=inet_addr("10.0.3.156");

    /* Request */
    strcpy(buffer,"SMTP REQUEST\n");
    sendto(sock,buffer,strlen(buffer),0,(struct sockaddr*)&server,sizeof(server));

    n=recvfrom(sock,buffer,MAX,0,NULL,NULL);
    buffer[n]='\0';
    printf("Server: %s",buffer);


    /* HELLO */
    strcpy(buffer,"HELLO client\n");
    sendto(sock,buffer,strlen(buffer),0,(struct sockaddr*)&server,sizeof(server));

    n=recvfrom(sock,buffer,MAX,0,NULL,NULL);
    buffer[n]='\0';
    printf("Server: %s",buffer);

    
    /* MAIL FROM */
    printf("Enter sender email: ");
    fgets(sender,MAX,stdin);
    sprintf(buffer,"MAIL FROM:%s",sender);

    sendto(sock,buffer,strlen(buffer),0,(struct sockaddr*)&server,sizeof(server));

    n=recvfrom(sock,buffer,MAX,0,NULL,NULL);
    buffer[n]='\0';
    printf("Server: %s",buffer);

    /* RCPT TO */
    printf("Enter receiver email: ");
    fgets(receiver,MAX,stdin);
    sprintf(buffer,"RCPT TO:%s",receiver);

    sendto(sock,buffer,strlen(buffer),0,(struct sockaddr*)&server,sizeof(server));

    n=recvfrom(sock,buffer,MAX,0,NULL,NULL);
    buffer[n]='\0';
    printf("Server: %s",buffer);

    /* DATA */
    strcpy(buffer,"DATA\n");
    sendto(sock,buffer,strlen(buffer),0,(struct sockaddr*)&server,sizeof(server));

    n=recvfrom(sock,buffer,MAX,0,NULL,NULL);
    buffer[n]='\0';
    printf("Server: %s",buffer);

    printf("Enter message (end with .):\n");

    while(1){
        fgets(msg,MAX,stdin);
        sendto(sock,msg,strlen(msg),0,(struct sockaddr*)&server,sizeof(server));
        if(strcmp(msg,".\n")==0)
            break;
    }

    /* QUIT */
    strcpy(buffer,"QUIT\n");
    sendto(sock,buffer,strlen(buffer),0,(struct sockaddr*)&server,sizeof(server));

    n=recvfrom(sock,buffer,MAX,0,NULL,NULL);
    buffer[n]='\0';
    printf("Server: %s",buffer);

    return 0;
}