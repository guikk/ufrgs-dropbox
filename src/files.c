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
		fprintf(stderr, "server: Cannot open file '%s\n'", path);
        exit(1);
	}

    fstat(fd, &st);

    packet p = {
        .length = st.st_size,
        ._payload = (char*) calloc(1, st.st_size)
    };

    read(fd, (void*) p._payload, p.length);
    close(fd);

    int bsent = 0;
    while (bsent < sizeof(packet)) {
        bsent += write(sockfd, &p + bsent, sizeof(packet) - bsent);

    }
}

void recv_file(int sockfd) {
    packet *p;
    int bread = 0;
    while (bread < sizeof(packet)) {
        bread += read(sockfd, p + bread, sizeof(packet) - bread);

    }
    if (!p) {
        fprintf(stderr, "Error reading file");
        return;
    }

    printf("Read packet of size %d, with this content:\n%s", p->length, p->_payload);
}