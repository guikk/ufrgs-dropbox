#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "config.h"

int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
	
    char buffer[256];
    if (argc < 2) {
		fprintf(stderr,"usage %s hostname\n", argv[0]);
		exit(0);
    }
	
	server = gethostbyname(argv[1]);
	if (server == NULL) {
        fprintf(stderr,"client: Error - no such host\n");
        exit(ERR_INVALID_HOST);
    }
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "client: Error opening socket\n");
        exit(ERR_SOCK_OPEN);
    }
    
	serv_addr.sin_family = AF_INET;     
	serv_addr.sin_port = htons(PORT);    
	serv_addr.sin_addr = *((struct in_addr *) server->h_addr);
	bzero(&(serv_addr.sin_zero), 8);     
	
    
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        fprintf(stderr, "client: Error connecting\n");
        exit(ERR_SOCK_CONN);
    }

    printf("Enter the message: ");
    bzero(buffer, 256);
    fgets(buffer, 256, stdin);
    
	/* write in socket */
	n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
		fprintf(stderr, "client: Error writing to socket\n");
        exit(ERR_SOCK_WRITE);
    }

    bzero(buffer,256);
	
	/* read from socket */
    n = read(sockfd, buffer, 256);
    if (n < 0) {
		fprintf(stderr, "client: Error reading from socket\n");
        exit(ERR_SOCK_READ);
    }

    printf("%s\n", buffer);
    
	close(sockfd);
    return 0;
}