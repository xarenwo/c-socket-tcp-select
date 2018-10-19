#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
 
#define PORT 3333
#define MAXLEN 255
void die(char *error);
 
int main(int argc, char const *argv[])
{
   int master_d;
   char sendbuff[MAXLEN], recvbuff[MAXLEN];
 
   struct sockaddr_in server_ip_port;
   int server_ip_port_length = sizeof(server_ip_port);
 
   memset(sendbuff, 0 , MAXLEN);
   memset(recvbuff, 0 , MAXLEN);
 
   master_d = socket(AF_INET, SOCK_STREAM, 0); // socket descriptor
   printf("Masterd %d \n",master_d);
   if(master_d < 0){
       die("Could not connect \n");
   }
   printf("Connected !\n");
 
 
 
   printf("Text to send : %s \n",sendbuff);
   server_ip_port.sin_family = AF_INET;
   server_ip_port.sin_addr.s_addr = inet_addr("127.0.0.1");
   server_ip_port.sin_port = htons(PORT);
 
 
   printf("Trying to connect !\n");
   if(connect(master_d, (struct sockaddr *)&server_ip_port, server_ip_port_length)<0){
        die("Could not connect to server !\n");
   }
   printf("Connected to server !\n");
 
 
   printf("Inserisci Stringa: ");
   gets(sendbuff);
   int bytesent = send(master_d , sendbuff , MAXLEN, 0);
   printf("%d bytes sent\n", bytesent);
   if(bytesent <= 0) {
       die("Could not send any bytes !");
 
   }
   printf("Bytes sent %d \n",bytesent);
   printf("Sent: %s \n",sendbuff);
 
   int bytesrecvd = recv(master_d, recvbuff , MAXLEN , 0);
 
   if(bytesrecvd <= 0) {
       die("Could not receive any bytes");
   }
   printf("Received %d bytes\n", bytesrecvd);
 
   printf("Received : %s\n",recvbuff);
   close(master_d);
   return 0;
 
}
 
 
void die(char * error){
    fprintf(stderr , "%s.\n", error);
    exit(1);
}
