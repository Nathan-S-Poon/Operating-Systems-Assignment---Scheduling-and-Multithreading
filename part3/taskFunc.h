#ifndef TASK_FUNC
#define TASK_FUNC
void printTasks(void* data);
void printGantt(void* data);
double aveWaitTime(LinkedList* tasks);
double aveTurnTime(LinkedList* tasks);
void incrementTime(LinkedList* list);
int isComplete(LinkedList* list);
Task* shortestJob(LinkedList* list, int time);
Task* highestPriority(LinkedList* list, int time);
#endif

#ifndef BOOLEAN
#define BOOLEAN
    #define FALSE 0
    #define TRUE !FALSE
#endif






