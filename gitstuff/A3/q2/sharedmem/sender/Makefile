all: sender exec

sender: sender.c
	gcc -o sender sender.c  

exec: sender
	./sender

clean:
	rm -f sender
