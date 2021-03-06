/****************************************************************
* Project 1: Mini HTTP Server
* Due 2/4/2015
*
* Server File
*
* Michael Kinkema
* Chase	Pietrangelo
* Danny	Selgo
* Daniel Jones
*
****************************************************************/

#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <string>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define BYTES_TO_SEND 256

using namespace std;

void* httpRequest(void* arg);

int main(int argc, char **argv){
	int port;
	string docroot;
	int p;

	if (argc > 1){
		for (int i = 1; i < argc; i++){
			if (strcmp(argv[i], "-p") == 0){
				i++;
				port = atoi(argv[i]);
			}
			else if (strcmp(argv[i], "-docroot") == 0){
				i++;
				docroot = argv[i];
			}
			else if (strcmp(argv[i], "-logfile") == 0){
				i++;
				freopen(argv[i], "w", stdout);
			}
			else{
				cerr << "Invalid option. Valid options are -p, -docroot, -logfile\n";
				return 1;
			}
		}
	}
	else{
		p = 8080;
		//getcwd(docroot);
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0){
		printf("Problem creating socket\n");
		return 1;
	}

	struct sockaddr_in serveraddr, clientaddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8080);
	serveraddr.sin_addr.s_addr = INADDR_ANY;

	bind(sockfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr));
	listen(sockfd, 10);

	char line[5000];
	while (1){
		pthread_t thread;
		void *result;
		int status;

		socklen_t len = sizeof(clientaddr);
		//g++ doesn't like this
		int clientsocket = accept(sockfd,(struct sockaddr*)&clientaddr,&len);
		   
		char line[5000];
		recv(clientsocket, line, 5000, 0);
		printf("Requested file from client: %s\n",line);

		//char str[INET_ADDRSTRLEN];
		//inet_ntop(AF_INET, &clientaddr, &str, INET_ADDRSTRLEN);
		//printf("A client connected (IP=%s : Port=9010)\n", str);

		if((status = pthread_create(&thread, NULL, httpRequest, &line)) != 0){
			fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
		}
	}

	return 0;
}

void* httpRequest(void* arg){
	//char line[5000];
	//int sockfd = *(int *) arg;
        //int n;

	cout << "Thread created" << endl;
/*
        while((n = recv(sockfd,line,5000,0))>0){
            printf("%s\n",line);
            memset(line,0,sizeof(line));
        }
        if(n<0){
            printf("Sorry, had a problem receiving.\n");
            exit(1);
        } else {
            printf("Server has disconnected.\n");
            exit(1);
        }
*/
	//close(sockfd);
	
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
		puts("Client connected");
		char line[5000];
		recv(clientsocket,line,5000,0);
		printf("Server received: %s\n",line);
		//send(clientsocket, line, 5000,0);
		FILE *fp = fopen("text.txt","rb");
		if(fp == NULL)
		{
			printf("Server: File read error\n");
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
				else if(ferror(fp))
					puts("Server: Error while reading file");
				break;
			}
		}
		close(clientsocket);
		fclose(fp);
	}
	
	
	
	pthread_detach(pthread_self());
	return 0;
}


