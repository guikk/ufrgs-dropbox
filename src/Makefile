all: clean build

build:
	@gcc -c server.c client.c
	@gcc -o server.out server.o
	@gcc -o client.out client.o

client:
	@./client.out guikk localhost 4000 # $(username) $(host) $(port)

server:
	@./server.out 4000 # $(port)

clean:
	@rm -rf *.o *.out