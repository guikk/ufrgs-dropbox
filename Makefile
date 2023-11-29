# Compiler
CC := gcc
# Compiler flags
CFLAGS := -Wall -Wextra -Iinclude -Icereal/include

# Source files
SERVER_SRC := src/server.c
CLIENT_SRC := src/client.c
FILES_SRC := src/files.c

# Object files
SERVER_OBJ := $(SERVER_SRC:.c=.o)
CLIENT_OBJ := $(CLIENT_SRC:.c=.o)
FILES_OBJ := $(FILES_SRC:.c=.o)

# Libraries
LIBS := -Llib -lcereal

# Output executables
SERVER_EXE := server
CLIENT_EXE := client

all: $(SERVER_EXE) $(CLIENT_EXE)

$(SERVER_EXE): $(SERVER_OBJ) $(FILES_OBJ)
	@$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(CLIENT_EXE): $(CLIENT_OBJ) $(FILES_OBJ)
	@$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Pattern rule for compiling source files
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

run_client:
	@./client guikk localhost 4000 # $(username) $(host) $(port)

run_server:
	@./server 4000 # $(port)

clean:
	@rm -f src/*.o $(SERVER_EXE) $(CLIENT_EXE)