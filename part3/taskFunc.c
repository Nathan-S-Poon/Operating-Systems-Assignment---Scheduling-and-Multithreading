#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "taskStruct.h"
#include "LinkedList.h"
#include "taskFunc.h"

void printTasks(void* data)
{
    Task* task;
    task = (Task*)data;
    printf("task: %d, arrival: %d, burst: %d, P: %d, ", task->taskNo, task->arrival, 
           task->burst, task->priority);
    printf("turn: %d, progress %d\n", task->turn, task->progress);
}

void printGantt(void* data)
{
    Gantt* gantt;
    gantt = (Gantt*)data;
    if(gantt->taskNo != -1)
    {
       printf("|P%d: %d-%d", gantt->taskNo, gantt->start, gantt->end);
    }
    else
    {
        printf("|none: %d-%d", gantt->start, gantt->end);
    }

}



double aveWaitTime(LinkedList* tasks)
{
    double time;
    double turntime;
    int numNodes;
    ListNode *node;
    Task* curTask;
    time = 0;
    numNodes = 0;
    node = tasks->head;
    while(node != NULL)
    {
        curTask = ((Task*)node->data);
        /*turnaround time = total time - arrival*/ 
        turntime = (curTask->turn - curTask->arrival);
        time = time + turntime - curTask->burst;
        numNodes++;
        node = node->next;
    }
    time = time/numNodes;
    return time;
}



double aveTurnTime(LinkedList* tasks)
{
    double time;
    int numNodes;
    ListNode *node;
    Task* curTask;
    time = 0;
    numNodes = 0;
    node = tasks->head;
    while(node != NULL)
    {
        curTask = ((Task*)node->data);
        /*turnaround time = total time - arrival*/ 
        time = time + (curTask->turn - curTask->arrival);
        numNodes++;
        node = node->next;
    }
    time = time/numNodes;
    return time;
}


/*Increment turnaround time*/
void incrementTime(LinkedList* list)
{
    ListNode* node;
    node = list->head;
    while(node != NULL)
    {
        if(((Task*)node->data)->progress != ((Task*)node->data)->burst)
        {
            ((Task*)node->data)->turn++;
        }
        node = node->next;
    }
}

/*returns job with least remaining time*/
Task* shortestJob(LinkedList* list, int time)
{
    ListNode* node;
    Task* curTask;
    Task* shortest;
    shortest = NULL;
    node = list->head;
    while((shortest == NULL)&&(node != NULL))
    {/*set shortest to an incomplete task that exists at time*/
        if((((Task*)node->data)->burst != ((Task*)(node->data))->progress)&&
          ((Task*)node->data)->arrival <= time)
        {
            shortest = (Task*)node->data;
        }
        node = node->next;
    }
    node = list->head;
    while((node != NULL)&&(shortest != NULL))
    {   /*first check if task is complete*/
        curTask = ((Task*)node->data);
        if((curTask->burst != curTask->progress)&&(curTask->arrival <= time))
        {
            if((curTask->burst - curTask->progress) < 
               (shortest->burst - shortest->progress))/*less remaining time*/
            {
                shortest = curTask;
            }/*if equal time remaining, pick recent*/
            else if((curTask->burst - curTask->progress) == 
                    (shortest->burst - shortest->progress))
            {
                if(curTask->arrival < shortest->arrival)
                {
                    shortest = curTask;
                }
            }
        }
        node = node->next;
    }
    return shortest;
}

Task* highestPriority(LinkedList* list, int time)
{
    ListNode* node;
    Task* highest;
    highest = NULL;
    node = list->head;
    while((highest == NULL)&&(node != NULL))
    {/*set highest to an incomplete task that exists at time*/
        if((((Task*)node->data)->burst != ((Task*)(node->data))->progress)&&
          ((Task*)node->data)->arrival <= time)
        {
            highest = (Task*)node->data;
        }
        node = node->next;
    }
    node = list->head;
    while((node != NULL)&&(highest != NULL))
    {   /*first check if task is complete*/
        if((((Task*)node->data)->burst != ((Task*)node->data)->progress)&&
          ((Task*)node->data)->arrival <= time)
        {
            if(((Task*)node->data)->priority < highest->priority)
            {
                highest = ((Task*)node->data);
            }
            else if(((Task*)node->data)->priority == highest->priority)
            {
                if(((Task*)node->data)->arrival < highest->arrival)
                {
                    highest = ((Task*)node->data);
                }
            }
        }
        node = node->next;
    }
    return highest;
}


/*check if all tasks are complete*/
int isComplete(LinkedList* list)
{
    ListNode* node;
    int complete;
    node = list->head;
    complete = TRUE;
    while(node != NULL)
    {
        if(((Task*)node->data)->progress != ((Task*)node->data)->burst)
        {
            complete = FALSE;
        }
        node = node->next;
    }
    return complete;
} 















































