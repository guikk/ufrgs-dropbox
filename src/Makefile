all: clean build

build:
	@gcc -c server.c client.c
	@gcc -o server.out server.o
	@gcc -o client.out client.o

client:
	@./client.out $(host)

server:
	@./server.out

clean:
	@rm -rf *.o *.out