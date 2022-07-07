#ifndef TASK_FUNC
#define TASK_FUNC
void printTasks(void* data);
void printGantt(void* data);
double aveWaitTime(LinkedList* tasks);
double aveTurnTime(LinkedList* tasks);
void incrementTime(LinkedList* list);
Task* highestPriority(LinkedList* list, int time);
int isComplete(LinkedList* list);

#endif

#ifndef BOOLEAN
#define BOOLEAN
    #define FALSE 0
    #define TRUE !FALSE
#endif






