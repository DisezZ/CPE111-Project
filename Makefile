#	Aplusplus CPE111 Final Project Makefile
#	From KMUTT	
#
#	Members:
#		Pataraphum	Chumaungphan	63070503437
#		Lutfee		Deemae			63070503448
#		Jidapa		Thongnirun		63070503462	
#		Purachet	Phanpuk			63070503466

CC = gcc
STD = -std=c99
ODIR = bin
SDIR = src
PROGRAM = 	main \
			linkedListQueue \
			linkedListNetworkMod \
		 	fileHandler \
	 		userInterface \
			dateCalendarManager \
			validateDate \
			dateFunctions
SOURCE = $(addsuffix .c,$(PROGRAM))
OBJECT = $(addsuffix .o,$(PROGRAM))
SRCPATH = $(addprefix src/,$(SOURCE))
OBJPATH = $(addprefix bin/,$(OBJECT))

EXECUTABLES = projectSchedule

.PHONY: clean all run

run: $(EXECUTABLES)
	./$(EXECUTABLES)

all: $(EXECUTABLES)

$(ODIR)/linkedListQueue.o :	$(SDIR)/linkedListQueue.c
	$(CC) -c -g $(STD) $(SDIR)/linkedListQueue.c -o $(ODIR)/linkedListQueue.o

$(ODIR)/linkedListNetworkMod.o	: $(SDIR)/linkedListNetworkMod.c
	$(CC) -c -g $(STD) $(SDIR)/linkedListNetworkMod.c -o $(ODIR)/linkedListNetworkMod.o

$(ODIR)/fileHandler.o	: $(SDIR)/fileHandler.c
	$(CC) -c -g $(STD) $(SDIR)/fileHandler.c -o $(ODIR)/fileHandler.o

$(ODIR)/userInterface.o	: $(SDIR)/userInterface.c
	$(CC) -c -g $(STD) $(SDIR)/userInterface.c -o $(ODIR)/userInterface.o

$(ODIR)/dateCalendarManager.o	: $(SDIR)/dateCalendarManager.c
	$(CC) -c -g $(STD) $(SDIR)/dateCalendarManager.c -o $(ODIR)/dateCalendarManager.o

$(ODIR)/validateDate.o	: $(SDIR)/validateDate.c
	$(CC) -c -g $(STD) $(SDIR)/validateDate.c -o $(ODIR)/validateDate.o

$(ODIR)/dateFunctions.o	: $(SDIR)/dateFunctions.c
	$(CC) -c -g $(STD) $(SDIR)/dateFunctions.c -o $(ODIR)/dateFunctions.o

$(ODIR)/main.o	: $(SDIR)/main.c
	$(CC) -c -g $(STD) $(SDIR)/main.c -o $(ODIR)/main.o

$(EXECUTABLES)	: $(OBJPATH)
	$(CC) -o $(EXECUTABLES) $(OBJPATH)

clean:
	- rm $(ODIR)/*.o
	- rm $(EXECUTABLES)

sclean:
	- rm %(ODIR)/*.o
