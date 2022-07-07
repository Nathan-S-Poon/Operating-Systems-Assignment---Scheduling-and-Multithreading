#variables
CC = gcc -pthread
CFLAGS = -Wall -pedantic -Werror -g 
EXEC = threadProgram 
OBJ = parent.o scheduler.o taskFunc.o LinkedList.o

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

LinkedList.o : LinkedList.h LinkedList.c
	$(CC) -c LinkedList.c $(CFLAGS)

taskFunc.o : LinkedList.h taskStruct.h taskFunc.h taskFunc.c
	$(CC) -c taskFunc.c $(CFLAGS)

scheduler.o : scheduler.c scheduler.h bufferStruct.h
	$(CC) -c scheduler.c $(CFLAGS)

parent.o : parent.c scheduler.h bufferStruct.h
	$(CC) -c parent.c $(CFLAGS) 

clean:
	rm -f $(EXEC) $(OBJ)
