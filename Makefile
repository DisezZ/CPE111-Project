#	Aplusplus CPE111 Final Project Makefile
#	From KMUTT	
#
#	Contributors:
#		
#
#
#

CC = gcc
STD = -std=c99
ODIR = bin
SDIR = src
PROGRAM = 	main \
			linkedListQueue \
			linkedListNetworkMod \
		 	fileHandler \
	 		userInterface
SOURCE = $(addsuffix .c,$(PROGRAM))
OBJECT = $(addsuffix .o,$(PROGRAM))
SRCPATH = $(addprefix src/,$(SOURCE))
OBJPATH = $(addprefix bin/,$(OBJECT))

EXECUTABLES = main

all:$(EXECUTABLES)

$(ODIR)/linkedListQueue.o :	$(SDIR)/linkedListQueue.c
	$(CC) -c $(STD) $(SDIR)/linkedListQueue.c -o $(ODIR)/linkedListQueue.o

$(ODIR)/linkedListNetworkMod.o	: $(SDIR)/linkedListNetworkMod.c
	$(CC) -c $(STD) $(SDIR)/linkedListNetworkMod.c -o $(ODIR)/linkedListNetworkMod.o

$(ODIR)/fileHandler.o	: $(SDIR)/fileHandler.c
	$(CC) -c $(STD) $(SDIR)/fileHandler.c -o $(ODIR)/fileHandler.o

$(ODIR)/userInterface.o	: $(SDIR)/userInterface.c
	$(CC) -c $(STD) $(SDIR)/userInterface.c -o $(ODIR)/userInterface.o

$(ODIR)/main.o	: $(SDIR)/main.c
	$(CC) -c $(STD) $(SDIR)/main.c -o $(ODIR)/main.o

$(EXECUTABLES)	: $(OBJPATH)
	$(CC) -o main $(OBJPATH)

clean:
	- rm $(ODIR)/*.o
	- rm $(EXECUTABLES)

sclean:
	- rm %(ODIR)/*.o