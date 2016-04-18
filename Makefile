all: cpusim

cpusim: cpusim.c ibarland-utils.o command-line-options.o jobQueue.o
	gcc -Wall cpusim.c -o cpuSim ibarland-utils.o command-line-options.o jobQueue.o -lm

command-line-options.o: command-line-options.c command-line-options.h
	gcc -Wall -c command-line-options.c

ibarland-utils.o: ibarland-utils.c ibarland-utils.h
	gcc -Wall -c ibarland-utils.c

jobQueue.o: jobQueue.c jobQueue.h
	gcc -Wall -c jobQueue.c

clean:
	rm -f cpuSim  *.o

run: cpusim
	./cpuSim