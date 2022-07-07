/*
*Program that implements the SRTF CPU scheduler
*Reads in file
*Uses LinkedList from UCP assignment submitted 25/5/2020
*
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "taskStruct.h"
#include "LinkedList.h"
#include "taskFunc.h"

int main()
{
    FILE* f;
    char* name;
    Task* task;
    LinkedList* taskList;
    LinkedList* order;
    int priority, arrive, burst, taskNo, progressDone, timer;
    Task* inProcess;
    Task* newProcess;
    Gantt* newGantt;
    listFunc printPtr;
    listFunc ganttPrint;
    inProcess = NULL;
    printPtr = &printTasks;
    ganttPrint = &printGantt;
    name = "a";
    while(strcmp(name, "QUIT") != 0)
    {     
        name = (char*)malloc(10*sizeof(char));
        printf("PP simulation:\n");
        scanf("%s", name);
        /*fgets(name, 10, stdin);*//*get fileinput*/    
        f = fopen(name, "r");
        if((f == NULL)&&(strcmp(name, "QUIT") != 0))
        {
            fprintf(stderr, "Error: file is null\n");
            /*strcpy(name, "QUIT");*/
        }    
        else if(strcmp(name, "QUIT") != 0)
        {
            taskNo = 1;
            taskList = createLinkedList();
            while(fscanf(f, "%d %d %d", &arrive, &burst, &priority) == 3)
            {
                task = (Task*)malloc(sizeof(Task));
                task->taskNo = taskNo;
                task->arrival = arrive;
                task->burst = burst;   
                task->priority = priority;
                task->turn = 0;
                task->progress = 0;
                taskNo++; 
                insertLast(taskList, task);         
                   
            }
            /*simulate SRTF scheduler*/
            timer = 0;
            progressDone = FALSE;
            inProcess = NULL;
            order = createLinkedList();/*for keeping track of process order*/
            if(listlength(taskList) != 0)
            {
                while(!progressDone)
                {
                    if(inProcess == NULL)
                    {
                        inProcess = shortestJob(taskList, timer);
                        newGantt = (Gantt*)malloc(sizeof(Gantt)); 
                        newGantt->taskNo = inProcess->taskNo;
                        newGantt->start = timer;
                    }
                    /*increment turnaround time for everything in list*/
                    incrementTime(taskList); 
                   
                    /*find next process if task is complete or shorter one exists*/ 
    
                    newProcess = shortestJob(taskList, timer); 
                    newProcess->progress++;
                    if(newProcess != inProcess)
                    {
                        inProcess = newProcess;
                        newGantt->end = timer;/*put old task in gantt chart*/
                        insertLast(order, newGantt);
                        newGantt = (Gantt*)malloc(sizeof(Gantt));/*start new gantt*/
                        newGantt->taskNo = inProcess->taskNo;
                        newGantt->start = timer;
                    }
                    
    
    
                    timer++; 
                    /*Check if all tasks are complete*/
                    progressDone = isComplete(taskList); 
                    if(progressDone)/*put last process in gantt*/
                    {
                        newGantt->end = timer;
                        insertLast(order, newGantt); 
                    }
                    printLinkedList(taskList, printPtr);
                 }
                    /*FOR TESTING*/
                    printLinkedList(taskList, printPtr);
                    printf("average turnaround time = %f\n", aveTurnTime(taskList));
                    printf("average waiting time = %f\n", aveWaitTime(taskList));
                    printf("gantt: ");
                    printLinkedList(order, ganttPrint);
                    printf("|\n");
                    /*free*/
                    freeLinkedList(taskList, &free);
                    freeLinkedList(order, &free);      
            }
            else/*If no valid lines in the file*/
            {
                freeLinkedList(taskList, &free);
                freeLinkedList(order, &free);  
                printf("Error: invalid file");  
            }

        }
    }
    free(name);
    
    return 0;
}



















