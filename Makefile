
all: cpusim

master: cpusim.c ibarland-utils.o command-line-options.o
	gcc -Wall cpusim.c -o cpusim ibarland-utils.o command-line-options.o -lm

command-line-options.o: command-line-options.c command-line-options.h ibarland-utils.o
	gcc -Wall -c command-line-options.c

ibarland-utils.o: ibarland-utils.c ibarland-utils.h
	gcc -Wall -c ibarland-utils.c

clean:
	rm -f cpusim  *.o

run: cpusim
	./cpusim