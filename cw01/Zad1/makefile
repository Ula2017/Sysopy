CC=gcc

static: Zadanie1.c
	$(CC) -c -Wall Zadanie1.c
	ar -crs libstatic1.a Zadanie1.o
	
shared: Zadanie1.c
	$(CC) -Wall -fPIC -c Zadanie1.c	
	$(CC) -shared -fPIC -o libshared1.so Zadanie1.o

cleanSt:
	rm libstatic1.a
	rm Zadanie1.o
cleanSh:
	rm libshared1.so
	rm Zadanie1.o
