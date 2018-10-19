
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define SERVICES 2
#define MAXLEN 255
#define LISTEN 5


void die(char *error);
void handle(int d);
void handleEcho(int d);


int main(int argc, char const *argv[]) {


    int sd[SERVICES];
    int port[SERVICES] = {3333,7772};
    struct sockaddr_in ip_port[SERVICES];
    int ip_port_len[SERVICES];
    int res;
    int acc;


    //SELECT

    fd_set originalfds, tempfds;
    struct timeval tv;
    int nOfReady;
    int maxD;

    FD_ZERO(&originalfds);

    //INIT
    maxD = 0;
    int i;
    for(i=0; i<SERVICES; i++){
        printf("Service %d\n",i);

        sd[i] = socket(AF_INET, SOCK_STREAM, 0);

        if(sd[i] < 0) {
            printf("Socket %d error\n", i);
            exit(1);
        }
        printf("Socket %d ok !\n", i);
        printf("MAXD %d\n",maxD);

        if(sd[i] > maxD) {
            maxD = sd[i];
        }


        FD_SET(sd[i], &originalfds); //settiamo il descrittore all'interno del nostro array original

        ip_port_len[i] = sizeof(ip_port[i]);
        memset(&ip_port[i],0, ip_port_len[i]);

        ip_port[i].sin_family = AF_INET;
        ip_port[i].sin_addr.s_addr = inet_addr("127.0.0.1");
        ip_port[i].sin_port = htons(port[i]);

        res = bind(sd[i], (struct sockaddr *)&ip_port[i],ip_port_len[i]);

        if(res<0){
            printf("Couldn't bind socket %d \n",i);
            exit(1);
        }
        printf("Socked %d binded !\n",i);

        res = listen(sd[i],LISTEN);

        if(res < 0){
            printf("Not listening on %d.\n",i);
            exit(1);
        }
        printf("Listening on socket %d\n",i);

    }

    while(1){
        // tempfds = originalfds
        memcpy(&tempfds, &originalfds, sizeof(originalfds));

        tv.tv_sec = 3;
        tv.tv_usec = 0;

        // calcolare mass descriptor

        //massimo di un vettore

        nOfReady = select(maxD + 1, &tempfds, NULL,NULL, &tv);

        if(nOfReady >0){

            if(FD_ISSET(sd[0], &tempfds)){

                acc = accept(sd[0], NULL,NULL);
                if(acc <0){
                    printf("Could not accept on socket 0\n");
                }
                printf("Accepting on socket 0\n");
                handle(acc);
                close(acc);
            }
            if(FD_ISSET(sd[1], &tempfds)){

                acc = accept(sd[1], NULL,NULL);
                if(acc <0){
                    printf("Could not accept on socket 1\n");
                }
                printf("Accepting on socket 1\n");
                handleEcho(acc);
                close(acc);
            }
        }else{
            printf("Timeout, retrying ...\n");
        }
    }
}

void handle(int d) {

    char recvbuff[MAXLEN];
    int sendint;
    // memset(sendbuff, 0, MAXLEN);
    memset(recvbuff, 0, MAXLEN);

    int bytesrecvd = recv(d, recvbuff, MAXLEN, 0);
    if(bytesrecvd <= 0) die("Could not receive ! \n");
    printf("Received %s\n",recvbuff);
    printf("Bytes received ! \n");

    sendint = (int)strlen(recvbuff);
    int sendintx;

    printf("Strlen %d \n",strlen(recvbuff));
    sendintx = htonl(sendint);
    printf("Decoding %d\n",ntohl(sendintx));
    printf("Sending %d\n",sendint);
    int bytesent = send(d, &sendintx, sizeof(sendintx), 0);

    if(bytesent <= 0){
        die("Could not reply to client !\n");
    }

    printf("Replied to client with %d bytes\n",bytesent);
    printf("Coded : %d\n",sendintx);

}

void handleEcho(int d) {

    char sendbuff[MAXLEN], recvbuff[MAXLEN];
    memset(sendbuff, 0, MAXLEN);
    memset(recvbuff, 0, MAXLEN);

    int bytesrecvd = recv(d, recvbuff, MAXLEN, 0);
    if(bytesrecvd <= 0) die("Could not receive ! \n");

    printf("Bytes received ! \n");

    strcpy(sendbuff,recvbuff);

    int bytesent = send(d, sendbuff, MAXLEN, 0);

    if(bytesent <= 0){
        die("Could not reply to client !\n");
    }

    printf("Replied to client with %d bytes\n",bytesent);
    printf("%s\n",sendbuff);


}

void die(char * error){
    fprintf(stderr , "%s.\n", error);
    exit(1);
}

