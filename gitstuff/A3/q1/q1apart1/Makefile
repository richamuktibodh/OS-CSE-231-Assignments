all: run exec

run: q1a.c
	gcc -o run q1a.c -l pthread

exec: run
	./run

clean:
	rm -f run