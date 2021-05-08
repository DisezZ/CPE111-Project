EXECUTABLES= projectSchedule

all:$(EXECUTABLES)

linkedListQueue.o :	linkedListQueue.c abstractQueue.h
	gcc -c -std=c99 linkedListQueue.c

linkedListNetworkMod.o	: linkedListNetworkMod.c abstractNetwork.h abstractQueue.h
	gcc -c -std=c99 linkedListNetworkMod.c

fileHandler.o	: fileHandler.c fileManagement.h abstractNetwork.h
	gcc -c -std=c99 fileHandler.c

userInterface.o	: userInterface.c userInterface.h
	gcc -c -std=c99 userInterface.c

main.o	: main.c main.h abstractNetwork.h fileManagement.h userInterface.h
	gcc -c -std=c99 main.c 

projectSchedule	: main.o linkedListNetworkMod.o linkedListQueue.o fileHandler.o userInterface.o
	gcc -o projectSchedule main.o  linkedListNetworkMod.o linkedListQueue.o fileHandler.o userInterface.o

clean:
	- rm *.o
	- rm $(EXECUTABLES)