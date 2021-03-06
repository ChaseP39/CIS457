#define BYTES_TO_SEND 1000

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in serveraddr, clientaddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9876);
	serveraddr.sin_addr.s_addr=INADDR_ANY;

	bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	listen(sockfd,10);
	while(1){
		int len = sizeof(clientaddr);
		int clientsocket = 
			accept(sockfd,(struct sockaddr*)&clientaddr,&len);
		
		char line[5000];
		recv(clientsocket,line,5000,0);
		printf("Server received: %s\n",line);
		//send(clientsocket, line, 5000,0);
		FILE *fp = fopen(line,"rb");
		if(fp == NULL)
		{
			perror("");
			return 1;
		}
		
		while(1)
		{
			char buff[BYTES_TO_SEND]={0};
			int bytesRead = fread(buff,1,BYTES_TO_SEND,fp);
			printf("Server: Bytes read %d\n",bytesRead);

			if(bytesRead > 0)
			{
				puts("Server: Sending... \n");
				send(clientsocket,buff,strlen(buff),0);
			}

			if(bytesRead < BYTES_TO_SEND)
			{
				if(feof(fp))
					puts("Server: Reached end of file");
				else if (ferror(fp))
					puts("Server: Error while reading file");
			}
		}
		close(clientsocket);
		fclose(fp);
	}

	return 0;
}
