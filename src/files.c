#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "files.h"

void send_file(int sockfd, char* path) {
	int fd;
    struct stat st;
	
	if ((fd = open(path, O_RDONLY)) == -1) {
		fprintf(stderr, "server: Cannot open file '%s'\n", path);
        exit(1);
	}

    fstat(fd, &st);

    packet p = {
        .length = st.st_size,
        ._payload = (char*) calloc(1, st.st_size)
    };

    read(fd, (void*) p._payload, p.length);
    close(fd);

    // send file length
    // write(sockfd,)

    int bsent = 0;
    bsent += write(sockfd, p._payload + bsent, p.length - bsent);
    // while (bsent < p.length) {
    //     bsent += write(sockfd, p._payload + bsent, p.length - bsent);
    // }
    printf("Sent %d/%d bytes from file %s\n", bsent, p.length, path);
} 

void recv_file(int sockfd) {
    char buffer[1024];
    int bread = 0;
    bread += read(sockfd, buffer + bread, 1024);
    // while (bread > 0) {
    //     bread += read(sockfd, buffer + bread, 1024);
    // }
    if (bread < 0) {
        fprintf(stderr, "Error reading file\n");
        return;
    }
    if (bread == 0) {
        fprintf(stderr, "Error reading file: connection closed\n");
        return;
    }

    printf("Read file of size %d, with this content:\n%s", bread, buffer);
}