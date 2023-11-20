all: clean build

build:
	@gcc -c server.c client.c
	@gcc -o server.out server.o
	@gcc -o client.out client.o

client:
	@./client.out $(username) $(host) $(port)

server:
	@./server.out $(port)

clean:
	@rm -rf *.o *.out