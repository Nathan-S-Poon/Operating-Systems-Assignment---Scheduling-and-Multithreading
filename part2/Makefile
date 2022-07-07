#variables
CC = gcc
CFLAGS = -Wall -pedantic -ansi -Werror -g
EXEC = SRTFScheduler  
OBJ = LinkedList.o SRTF.o taskFunc.o

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

LinkedList.o : LinkedList.h LinkedList.c 
	$(CC) -c LinkedList.c $(CFLAGS)

taskFunc.o : LinkedList.h taskStruct.h taskFunc.h taskFunc.c
	$(CC) -c taskFunc.c $(CFLAGS)

SRTF.o : SRTF.c taskFunc.h LinkedList.h taskStruct.h 
	$(CC) -c SRTF.c $(CFLAGS) 

clean:
	rm -f $(EXEC) $(OBJ)
