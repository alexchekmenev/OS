all: bufcat

bufcat: bufcat.c
	gcc -w -L../lib -Wl,-rpath=../lib -o bufcat bufcat.c -lbufio

clean:
	rm *.o bufcat
