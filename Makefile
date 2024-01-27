# Compiler
CC := gcc
CXX := g++
# Compiler flags
CFLAGS := -Wall -Wextra -Iinclude -Ilib/cereal-1.3.2/include -std=c++11 -pthread

# Source files
SRC = src
SERVER_SRC := $(SRC)/server.cpp
CLIENT_SRC := $(SRC)/client.cpp
FILES_SRC := $(SRC)/files.cpp

# Executables
SERVER_EXE := server
CLIENT_EXE := client

# Libraries
LIBS := -Llib -lcereal

all: $(SERVER_EXE) $(CLIENT_EXE)

$(SERVER_EXE): $(SERVER_SRC) $(FILES_SRC)
	$(CXX) $(CFLAGS) -o $@ $^

$(CLIENT_EXE): $(CLIENT_SRC) $(FILES_SRC)
	$(CXX) $(CFLAGS) -o $@ $^

username ?= guikk
host ?= localhost
port ?= 4000
run_client:
	@./client $(username) $(host) $(port)

run_server:
	@./server $(port)

clean:
	@rm -f $(SRC)/*.o $(SERVER_EXE) $(CLIENT_EXE)