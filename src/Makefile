all: clean build

build:
	@gcc -c *.c
	@gcc -o server.out server.o files.o
	@gcc -o client.out client.o files.o

client:
	@./client.out guikk localhost 4000 # $(username) $(host) $(port)

server:
	@./server.out 4000 # $(port)

clean:
	@rm -rf *.o *.out