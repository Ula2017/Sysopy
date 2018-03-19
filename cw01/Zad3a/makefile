CC=gcc
CFLAGS= -Wall -pedantic -std=c11

static: Zadanie1.c test.c
	$(CC) -Wall -c Zadanie1.c
	ar -crs libstatic1.a Zadanie1.o
	$(CC) -Wall test.c -o test1 -L. -lstatic1
	./test1

shared: Zadanie1.c test.c
	$(CC) $(CFLAGS) -fPIC -c Zadanie1.c
	$(CC) $(CFLAGS) -shared -fPIC -o libshared1.so Zadanie1.o
	$(CC) $(CFLAGS) test.c -o test1 -L. -Wl,-rpath=. -lshared1
	./test1
	
dynamic: Zadanie1.c test.c
	$(CC) -Og -Wall -fPIC -c Zadanie1.c
	$(CC) -shared -fPIC -o libdynamic1.so Zadanie1.o
	$(CC) -Og -Wall -L. -Wl,-rpath=. ./test.c -ldynamic1 -o ./maind.c -ldl
	./maind.c

	
cleanSt:
	rm libstatic1.a
	rm Zadanie1.o
	rm test1

cleanSh:
	rm libshared1.so
	rm Zadanie1.o
	rm test1
cleanDy:
	rm libdynamic1.so
	rm Zadanie1.o
	rm maind.c

	
