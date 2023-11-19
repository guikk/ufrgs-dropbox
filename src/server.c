#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "config.h"

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, n;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "server: Error opening socket");
		exit(ERR_SOCK_OPEN);
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);     
    
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "server: Error on binding");
		exit(ERR_SOCK_BIND);
	}

	printf("Server listening on %s:%d\n", inet_ntoa(serv_addr.sin_addr), PORT);
	
	listen(sockfd, 5);
	
	clilen = sizeof(struct sockaddr_in);
	if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1) {
		fprintf(stderr, "server: Error on accept");
		exit(ERR_SOCK_ACCEPT);
	}
	
	bzero(buffer, 256);
	
	/* read from socket */
	n = read(newsockfd, buffer, 256);
	if (n < 0) {
		fprintf(stderr, "server: Error reading from socket\n");
        exit(ERR_SOCK_READ);
    }
	printf("client: %s\n", buffer);
	
	/* write in socket */ 
	n = write(newsockfd, "server: I got your message", 26);
	if (n < 0) {
		fprintf(stderr, "server: Error writing to socket\n");
        exit(ERR_SOCK_WRITE);
    }

	close(newsockfd);
	close(sockfd);
	return 0; 
}