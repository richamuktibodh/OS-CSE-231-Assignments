all: client exec

client: client.c
	gcc -o client client.c -l pthread 

exec: client
	./client ../server/mysocket

clean:
	rm -f client