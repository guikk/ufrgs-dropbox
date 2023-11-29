#ifndef FILES_H
#define FILES_H

#include <stdint.h>

typedef struct packet{
uint16_t type;
uint16_t seqn;
uint32_t total_size;
uint16_t length;
const char* _payload;
} packet;

void send_file(int sockfd, char* path);
void recv_file(int sockfd);

#endif