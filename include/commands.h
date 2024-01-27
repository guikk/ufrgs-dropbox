#ifndef COMMANDS_H
#define COMMANDS_H

void upload_file(char* path);
char* download_file(char* filename);
void delete_file(char* filename);

char** list_server(void);
char** list_client(void);

void get_sync_dir(void);

void exit_session(void);

#endif