#include <netinet/in.h> 
#include <netdb.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <string.h> 
#include <stdio.h>
#include <sys/socket.h> 
#include <sys/types.h> 
#include <ctype.h>
#include <unistd.h>

#define PORT 11111
#define SA struct sockaddr

int socketConnect(char *host, in_port_t port){
    struct hostent *hp;
    struct sockaddr_in addr;
    int on=1, sock;
    if ((hp = gethostbyname(host))==NULL){
        herror("gethostbyname");
        exit(1);
    }
	bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_port = htons(port);
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));


    if(sock ==-1){
        perror("setsockopt");
        exit(1);
    }

    if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1){
        perror("connect");
        exit(1);
    }
    return sock;
}

void server(int connfd){
    char buff[255];
    char buff2[1024];
    int osockfd, count=0;
    int file, cachefilefd; 
    char* urls[10];
    cachefilefd = open("urls.txt", O_RDWR | O_CREAT | O_TRUNC);
    for(;;){
        bzero(buff, sizeof(buff));
        read(connfd, buff, sizeof(buff));
        if(count < 10){
            urls[count] = buff;
        }
        else{
            for(int i=0; i<9; i++){
               urls[i] = urls[i+1]; 
            }
            urls[9]= buff;
        }

        osockfd = socketConnect(buff, osockfd);
        write(osockfd, "GET /\r\n", strlen("GET /\r\n"));
        bzero(buff2, sizeof(buff2));
        while(read(osockfd, buff2, sizeof(buff2))!=0){
            fprintf(stderr, "%s", buff2);
            bzero(buff2, sizeof(buff));
        }

        file = open(buff, O_CREAT | O_RDWR | O_TRUNC );
        write(cachefilefd, buff, sizeof(buff));
        write(file, buff2, sizeof(buff2));
        write(connfd, buff2, sizeof(buff2));
        count++;
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
		printf("Socket created successfully\n");
	}

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(PORT);

    if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) !=0){
        printf("socket bind failed");
        exit(0);
    }
    else{
        printf("socket bound successfully\n");
    }

    if(listen(sockfd, 5) != 0){
        exit(0);
    }
    else{
        printf("listening on port: %d\n", PORT);
    }

    int len = sizeof(cli);

	connfd = accept(sockfd, (SA*)&cli, &len);
	if(connfd <0){
		printf("Server accept failed\n");
		exit(0);
	}
	server(connfd);

    return 0;
}
