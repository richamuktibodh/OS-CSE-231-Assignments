all: server exec

server: server.c
	gcc -o server server.c -l pthread 

exec: server
	./server

clean:
	rm -f server