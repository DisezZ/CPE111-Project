EXECUTABLES= projectSchedule

all:$(EXECUTABLES)

linkedListQueue.o :	linkedListQueue.c abstractQueue.h
	gcc -c linkedListQueue.c

networkHandler.o	: networkHandler.c abstractNetwork.h abstractQueue.h
	gcc -c networkHandler.c

fileManagement.o	: fileHandler.c fileManagement.h abstractNetwork.h
	gcc -c fileHandler.c

main.o	: main.c
	gcc -c main.c

projectSchedule	: main.o networkHandler.o linkedListQueue.o fileHandler.o
	gcc -o projectSchedule main.o  networkHandler.o linkedListQueue.o fileHandler.o

clean:
	- rm *.o
	- rm $(EXECUTABLES)