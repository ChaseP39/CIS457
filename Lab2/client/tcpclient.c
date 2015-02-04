#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  int sockfd = socket(AF_INET,SOCK_STREAM,0);
  if (sockfd < 0) {
    printf("There was a problem creating the socket\n");
    return 1;
  }
  
  struct sockaddr_in serveraddr;
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(9876);
  serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  int e = connect(sockfd,(struct sockaddr*)&serveraddr,
		  sizeof(struct sockaddr_in));
  
  if (e < 0) {
    printf("Thre were some problems with connecting\n");
    return 1;
  }
  
  printf("Enter a filename: ");
  char line[5000];
  //char line2[5000];
  fgets(line,5000,stdin);
  send(sockfd,line,strlen(line),0);
  
  File *recFile;
  int n = recv(sockfd,recFile,sizeof(recFile),0);
  if (n < 0) {
    printf("Sorry, I had a problem receiving\n");
    return 1;
  }
  
  remove(line);
  
  
  
  printf("Got from the server %s\n", line);
  close(sockfd);
  
  return 0;
}
