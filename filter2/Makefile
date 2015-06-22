all: filter2

filter2: filter2.c
	gcc -w -L../lib -Wl,-rpath=../lib -o filter2 filter2.c -lhelpers

clean:
	rm *.o filter2
