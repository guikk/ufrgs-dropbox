#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "errors.h"
#include "files.h"

int start(int port);
int accept_client(int sockfd);
void read_from(int sockfd);
void write_to(int sockfd, char* message);

char buffer[256];

int main(int argc, char *argv[])
{
	int sockfd, newsockfd;
	struct sockaddr_in serv_addr;
	
	if (argc < 2) {
		fprintf(stderr,"usage ./%s <port> \n", argv[0]);
		exit(0);
    }

	int port = atoi(argv[1]);

	sockfd = start(port);
	
	newsockfd = accept_client(sockfd);
	
	bzero(buffer, 256);
	
	recv_file(sockfd);
	// read_from(newsockfd);
	
	write_to(newsockfd, "server: I got your message");

	close(newsockfd);
	close(sockfd);
	return 0;
}

int start(int port) {
	int sockfd;
	struct sockaddr_in serv_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "server: Error opening socket");
		exit(ERR_SOCK_OPEN);
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);     
    
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "server: Error on binding");
		exit(ERR_SOCK_BIND);
	}
	
	listen(sockfd, 5);
	printf("Server listening on port %d\n", port);

	return sockfd;
}

int accept_client(int sockfd) {
	int newsockfd;
	socklen_t clilen;
	struct sockaddr_in cli_addr;

	clilen = sizeof(struct sockaddr_in);
	if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1) {
		fprintf(stderr, "server: Error on accept");
		exit(ERR_SOCK_ACCEPT);
	}

	return newsockfd;
}

void read_from(int sockfd) {
    int n;
    bzero(buffer,256);

    n = read(sockfd, buffer, 256);
    if (n < 0) {
		fprintf(stderr, "server: Error reading from socket\n");
        exit(ERR_SOCK_READ);
    }

    printf("server: Read %d bytes from socket %d\n< %s\n", n, sockfd, buffer);
}

void write_to(int sockfd, char* message) {
    int n;
	n = write(sockfd, message, strlen(message));
    if (n < 0) {
		fprintf(stderr, "server: Error writing to socket %d\n", sockfd);
        exit(ERR_SOCK_WRITE);
    }

    printf("server: Sent %d bytes to socket %d\n", n, sockfd);
}
