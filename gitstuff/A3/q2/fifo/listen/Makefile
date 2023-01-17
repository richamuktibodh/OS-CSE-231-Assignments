all: listen exec

listen: listen.c
	gcc -o listen listen.c 

exec: listen
	./listen

clean:
	rm -f listen