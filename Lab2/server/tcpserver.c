#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
 int sockfd = socket(AF_INET,SOCK_STREAM,0);
 
 struct sockaddr_in serveraddr, clientaddr;
 serveraddr.sin_family = AF_INET;
 serveraddr.sin_port = htons(9876);
 serveraddr.sin_addr.s_addr = INADDR_ANY;
 
 bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
 listen(sockfd,10);
 while(1) {
   int len = sizeof(clientaddr);
   int clientsocket = accept(sockfd,(struct sockaddr*)&clientaddr,&len);
   char line[5000];
   recv(clientsocket,line,5000,0);
   
   FILE * file = fopen(line, "r");
   printf("Requested file from client: %s\n",line);
   
   if (file != NULL) {
     send(clientsocket,file,sizeof(file),0);
     
   } else { // If file doesn't exist
      char errorMsg[] = "The file does not exist";
      send(clientsocket,errorMsg,strlen(errorMsg),0);
      
   }
   
   close(clientsocket);
 }
 
 return 0;
}