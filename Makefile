all: microcat hw1b hw1a clean

microcat: microcat.o
	gcc -o microcat microcat.o -lcheck

hw1b: hw1b.o
	gcc -o hw1b hw1b.o -lcheck

hw1a: hw1a.o
	gcc -o hw1a hw1a.o -lcheck

microcat.o: microcat.c
	gcc -c microcat.c

hw1b.o: hw1b.c
	gcc -c hw1b.c

hw1a.o: hw1a.c
	gcc -c hw1a.c

clean:
	rm -rf *.o *.gch *.dSYM
