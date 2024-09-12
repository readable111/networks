#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <ctype.h>
#include <unistd.h> // read(), write(), close()
#define PORT 8080 
#define SA struct sockaddr 
#define MAX 255

void func(int connfd){
	char buff[MAX];
	int n;
	for(;;){
		bzero(buff, MAX);

		read(connfd, buff, sizeof(buff));
		printf("From client: %s\t To client: ", buff);
		for(int i=0;i<sizeof(buff)/sizeof(char); i++){
			if(isalpha(buff[i])){
				buff[i] = toupper(buff[i]);
			}
		}
		write(connfd, buff, sizeof(buff));
		if(strncmp("exit", buff, 4) == 0){
			printf("Server exit");
			break;
		}
	}
}


int main(int argc, char * argv[]){
	int connfd, sockfd, len;
	struct sockaddr_in servaddr, cli;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		printf("Failed");
		exit(0);
	}
	else{
		printf("Socket created successfully");
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) !=0){
		printf("socket bind failed");
		exit(0);
	}

	if((listen(sockfd, 5)) !=0){
		printf("Listen Failed");
		exit(0);
	}

	else{
		printf("Server Listening");
	}
	len = sizeof(cli);

	connfd = accept(sockfd, (SA*)&cli, &len);
	if(connfd <0){
		printf("Server accept failed");
		exit(0);
	}

	else printf("server accept the client");
	func(connfd);

	close(sockfd);


}
