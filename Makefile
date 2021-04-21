EXECUTABLES= networkHandler 

all:$(EXECUTABLES)

linkedListQueue.o :	linkedListQueue.c abstractQueue.h
	gcc -c linkedListQueue.c

minPriorityQueue.o :	minPriorityQueue.c minPriorityQueue.h
	gcc -c minPriorityQueue.c

networkHandler.o	: networkHandler.c abstractNetwork.h abstractQueue.h
	gcc -c networkHandler.c

networkHandler	: networkHandler.o linkedListQueue.o
	gcc -o networkHandler networkHandler.o linkedListQueue.o

clean:
	- rm *.o
	- rm $(EXECUTABLES)