#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>


FILE *fp;
char fname[100], dest[100];

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

    read(cltsoc, fname, sizeof(fname));
    fp = fopen(fname, "r");

    read(cltsoc, dest, sizeof(dest));
    FILE *d = fopen(dest, "w");

    printf("\nFile name from client: %s", fname);
    while(fgets(msg, sizeof(msg), fp) != NULL){
        fwrite(msg, sizeof(msg), strlen(msg), d);
        write(cltsoc, msg, sizeof(msg));
    }

    printf("\nFile sent to client!!\n");

    close(soc);
    close(cltsoc);

    fclose(fp);
    fclose(d);

}