CC=gcc

generate: zad1.c
	$(CC) -Wall zad1.c -o zad1
	./zad1 sys generate test.txt 100 512

shuffleSys: zad1.c
	$(CC) -Wall zad1.c -o zad1
	./zad1 sys shuffle test.txt 100 512

shuffleLib: zad1.c
	$(CC) -Wall zad1.c -o zad1
	./zad1 lib shuffle test.txt 100 512

sortSys: zad1.c
	$(CC) -Wall zad1.c -o zad1
	./zad1 sys sort test.txt 100 512

sortLib: zad1.c
	$(CC) -Wall zad1.c -o zad1
	./zad1 lib sort test.txt 100 512
	
cleanR: 
	rm zad1
	rm test.txt
	rm test1.txt
	rm test2.txt
	rm results.txt
			
result: zad1.c
	$(CC) -Wall zad1.c -o zad1
	echo "" >> results.txt
	./zad1 sys generate test.txt 500 4 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys shuffle test1.txt 500 4 >> results.txt
	./zad1 lib shuffle test2.txt 500 4 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys sort test1.txt 500 4 >> results.txt
	./zad1 lib sort test2.txt 500 4 >> results.txt

	./zad1 sys generate test.txt 2000 4 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys shuffle test1.txt 2000 4 >> results.txt
	./zad1 lib shuffle test2.txt 2000 4 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys sort test1.txt 2000 4 >> results.txt
	./zad1 lib sort test2.txt 2000 4 >> results.txt

	./zad1 sys generate test.txt 2000 512 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys shuffle test1.txt 2000 512 >> results.txt
	./zad1 lib shuffle test2.txt 2000 512 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys sort test1.txt 2000 512 >> results.txt
	./zad1 lib sort test2.txt 2000 512 >> results.txt

	./zad1 sys generate test.txt 500 512 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys shuffle test1.txt 500 512 >> results.txt
	./zad1 lib shuffle test2.txt 500 512 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys sort test1.txt 500 512 >> results.txt
	./zad1 lib sort test2.txt 500 512 >> results.txt

	./zad1 sys generate test.txt 500 4096 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys shuffle test1.txt 500 4096 >> results.txt
	./zad1 lib shuffle test2.txt 500 4096 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys sort test1.txt 500 4096 >> results.txt
	./zad1 lib sort test2.txt 500 4096 >> results.txt

	./zad1 sys generate test.txt 2000 4096 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys shuffle test1.txt 2000 4096 >> results.txt
	./zad1 lib shuffle test2.txt 2000 4096 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys sort test1.txt 2000 4096 >> results.txt
	./zad1 lib sort test2.txt 2000 4096 >> results.txt

	./zad1 sys generate test.txt 500 8192 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys shuffle test1.txt 500 8192 >> results.txt
	./zad1 lib shuffle test2.txt 500 8192 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys sort test1.txt 500 8192 >> results.txt
	./zad1 lib sort test2.txt 500 8192 >> results.txt

	./zad1 sys generate test.txt 2000 8192 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys shuffle test1.txt 2000 8192 >> results.txt
	./zad1 lib shuffle test2.txt 2000 8192 >> results.txt
	cp test.txt test1.txt
	cp test.txt test2.txt
	./zad1 sys sort test1.txt 2000 8192 >> results.txt
	./zad1 lib sort test2.txt 2000 8192 >> results.txt
	
	

	
	

