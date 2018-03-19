compile:
	gcc -Wall master.c -o master
	gcc -Wall slave.c -lm -o slave

clean:
	rm master
	rm slave
	rm data

run:
	./master pipe 100
