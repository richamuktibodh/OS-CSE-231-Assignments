all: run exec

run: q2b.c
	gcc -o run q2b.c -l pthread 

exec: run
	./run

clean:
	rm -f run