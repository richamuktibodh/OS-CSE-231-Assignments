all: run exec

run: q2a.c
	gcc -o run q2a.c -l pthread 

exec: run
	./run

clean:
	rm -f run