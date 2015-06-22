all: filter

filter: filter.c
	gcc -w -L../lib -Wl,-rpath=../lib -o filter filter.c -lhelpers

clean:
	rm *.o filter
