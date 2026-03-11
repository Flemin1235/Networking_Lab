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

    soc = socket(AF_INET, SOCK_STREAM, 0);
    
    servadd.sin_family = AF_INET;
    servadd.sin_port = htons(port);
    servadd.sin_addr.s_addr = INADDR_ANY;

    if(bind(soc, (struct sockaddr *)&servadd, len) != -1){
        printf("\nSocket Binded!!");
        listen(soc, 5);
    };



    cltsoc = accept(soc, (struct sockaddr *)&cltadd, &len);

    if(cltsoc != -1){
        printf("\nConnected!!");
    };

    read(cltsoc, &num, sizeof(num));

    printf("\nMessage from client: %d", num);
    if(isprime(num)){
        write(cltsoc, "It is Prime No\n", sizeof("It is Prime No"));
    }
    else{
        write(cltsoc, "It is Not Prime No\n", sizeof("It is Not Prime No"));
        
    }
    printf("\nMessage sent to client!!\n");

    close(soc);
    close(cltsoc);

}