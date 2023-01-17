all: speak exec

speak: speak.c
	gcc -o speak speak.c 

exec: speak
	./speak

clean:
	rm -f speak