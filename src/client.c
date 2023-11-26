#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "errors.h"

int connect_to_server(struct hostent *server, int port);
void read_from(int sockfd);
void write_to(int sockfd, char* message);

char buffer[256];

int main(int argc, char *argv[])
{
    int sockfd;

    char username[50];
    struct hostent *server;
    int port;
	
    if (argc < 4) {
		fprintf(stderr,"usage ./%s <username> <server_ip_address> <port> \n", argv[0]);
		exit(0);
    }

    strcpy(username, argv[1]);

	server = gethostbyname(argv[2]);
	if (server == NULL) {
        fprintf(stderr,"client: Error - no such host\n");
        exit(ERR_INVALID_HOST);
    }

    port = atoi(argv[3]);
	
    sockfd = connect_to_server(server, port);

    printf("> ");
    bzero(buffer, 256);
    fgets(buffer, 256, stdin);
    
    write_to(sockfd, buffer);

    read_from(sockfd);
    
	close(sockfd);
    return 0;
}

int connect_to_server(struct hostent *server, int port) {
    int sockfd;
    struct sockaddr_in serv_addr;

	serv_addr.sin_family = AF_INET;   
	serv_addr.sin_port = htons(port);   
	serv_addr.sin_addr = *((struct in_addr *) server->h_addr);
	bzero(&(serv_addr.sin_zero), 8);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "client: Error opening socket\n");
        exit(ERR_SOCK_OPEN);
    } 
    
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        fprintf(
            stderr,
            "client: Error connecting to socket on %s:%d\n",
            inet_ntoa(serv_addr.sin_addr), port
        );
        exit(ERR_SOCK_CONN); 
    }

    printf(
        "client: Connected to %s:%d through socket %d\n",
        inet_ntoa(serv_addr.sin_addr), port, sockfd
    );
    return sockfd;
}

void read_from(int sockfd) {
    int n;
    bzero(buffer,256);

    n = read(sockfd, buffer, 256);
    if (n < 0) {
		fprintf(stderr, "client: Error reading from socket\n");
        exit(ERR_SOCK_READ);
    }

    printf("client: Read %d bytes from socket %d\n< %s\n", n, sockfd, buffer);
}

void write_to(int sockfd, char* message) {
    int n;
	n = write(sockfd, message, strlen(message));
    if (n < 0) {
		fprintf(stderr, "client: Error writing to socket %d\n", sockfd);
        exit(ERR_SOCK_WRITE);
    }

    printf("client: Sent %d bytes to socket %d\n", n, sockfd);
}
