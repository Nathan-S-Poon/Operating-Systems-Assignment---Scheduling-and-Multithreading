#variables
CC = gcc
CFLAGS = -Wall -pedantic -Werror -g
EXEC = PPScheduler  
OBJ = LinkedList.o ppScheduler.o taskFunc.o

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

LinkedList.o : LinkedList.h LinkedList.c 
	$(CC) -c LinkedList.c $(CFLAGS)

taskFunc.o : LinkedList.h taskStruct.h taskFunc.h taskFunc.c
	$(CC) -c taskFunc.c $(CFLAGS)

ppScheduler.o : ppScheduler.c taskFunc.h LinkedList.h taskStruct.h 
	$(CC) -c ppScheduler.c $(CFLAGS) 

clean:
	rm -f $(EXEC) $(OBJ)
