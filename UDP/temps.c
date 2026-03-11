#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>

int isprime(int num){
    int c = 0;
    for(int i = num; i >= 1; i--){
        if(num % i == 0){
            c++;
        }
    }

    return (c == 2);
}


int main(){
    int soc, cltsoc, port = 8888, num;
    struct sockaddr_in servadd, cltadd;
    socklen_t len = sizeof(cltadd);
    char msg[100];

    soc = socket(AF_INET, SOCK_DGRAM, 0);
    
    servadd.sin_family = AF_INET;
    servadd.sin_port = htons(port);
    servadd.sin_addr.s_addr = INADDR_ANY;

    if(bind(soc, (struct sockaddr *)&servadd, len) != -1){
        printf("\nSocket Binded!!");
    };

    recvfrom(soc, &num, sizeof(num), 0, (struct sockaddr *)&cltadd, &len);

    printf("\nNumber from client: %d", num);

    if(isprime(num)){
        sendto(soc, "Prime No", sizeof("Prime No"), 0, (struct sockaddr *)&cltadd, len);
    }

    else{
        sendto(soc, "Not a Prime No", sizeof("Not a Prime No"), 0, (struct sockaddr *)&cltadd, len);
    }

    printf("\nResult sent to client!!");
    close(soc);

}