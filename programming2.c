#include <netinet/in.h> 
#include <netdb.h>
#include <stdlib.h> 
#include <string.h> 
#include <stdio.h>
#include <sys/socket.h> 
#include <sys/types.h> 
#include <ctype.h>
#include <unistd.h>
#define PORT 80
#define SA struct sockaddr 

void server(int connfd){
    char buff[255];
    char buff2[1024];
    int osockfd;
    for(;;){
        bzero(buff, sizeof(buff));
        read(connfd, buff, sizeof(buff));
        osockfd = socket_connect(osockfd, buff, sizeof(buff));
        write(osockfd, "GET /\r\n", strlen("GET /\r\n"));
        bzero(buff2, sizeof(buff2));
        while(read(osockfd, buff2, sizeof(buff2))!=0){
            fprintf(stderr, "%s", buff2);
            bzero(buff2, sizeof(buff));
        }
        write(connfd, buff2, sizeof(buff2));
    }
    shutdown(osockfd, SHUT_RDWR);
    close(osockfd);
}


int main(){
    int connfd, sockfd;
    struct sockaddr_in serveraddr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
		printf("Failed");
		exit(0);
	}
	else{
		printf("Socket created successfully");
	}

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	

    if(bind(sockfd, (SA*)&serveraddr, sizeof(serveraddr))!=0){
        printf("socket bind failed");
        exit(0);
    }
    else{
        printf("socket bound successfully");
    }

    if(listen(sockfd,5) != 0){
        printf("listen failed");
        exit(0);
    }
    else{
        printf("listening on port: %d", PORT);
    }

    int len = sizeof(cli);
	connfd = accept(sockfd, (SA*)&cli, &len);
	if(connfd <0){
		printf("Server accept failed");
		exit(0);
	}




    return 0;
}