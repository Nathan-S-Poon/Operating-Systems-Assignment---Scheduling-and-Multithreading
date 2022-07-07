/*
*Program that implements the priority preemptive CPU scheduler
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
    listFunc ganttPrint;
    inProcess = NULL;
    /*listFunc = printPtr = &printTasks;*/
    ganttPrint = &printGantt;
    name = (char*)malloc(10*sizeof(char));
    strcpy(name, "a");

    while(strcmp(name, "QUIT") != 0)
    {     
        printf("SRTF simulation:\n");
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
                /*Validate data, ignore*/
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
             
            /*simulate Priority scheduler*/
            timer = 0;
            progressDone = FALSE;
            inProcess = NULL;
            order = createLinkedList();/*for keeping track of process order*/
            if(listLength(taskList) != 0)
            {

                if(inProcess == NULL)/*For the start of the process*/
                {   /*Get first process*/
                    inProcess = shortestJob(taskList, timer);
                    /*If no current available process*/
                    if(inProcess == NULL)
                    {
                        newGantt = (Gantt*)malloc(sizeof(Gantt)); 
                        newGantt->taskNo = -1;
                        newGantt->start = timer;
                    }
                    while(inProcess == NULL && (!isComplete(taskList)))
                    {
                        timer++;
                        incrementTime(taskList);
                        inProcess = shortestJob(taskList, timer);
                        if(inProcess != NULL)
                        {
                            newGantt->end = timer;
                            insertLast(order, newGantt);
                        }
                    }
                    newGantt = (Gantt*)malloc(sizeof(Gantt)); 
                    newGantt->taskNo = inProcess->taskNo;
                    newGantt->start = timer;
                    progressDone = isComplete(taskList); 
 
                }
                while(!progressDone)
                {
                   /*increment turnaround time for everything in list*/
                    incrementTime(taskList); 
                    /*increment processTime for pointer*/
                    newProcess = shortestJob(taskList, timer);
                    /*If no current available process*/
                    if(newProcess == NULL)
                    {
                        newGantt->end = timer;/*put old task in gantt chart*/
                        insertLast(order, newGantt); 
                        newGantt = (Gantt*)malloc(sizeof(Gantt)); 
                        newGantt->taskNo = -1;
                        newGantt->start = timer;
                    }
                    while(newProcess == NULL && (!isComplete(taskList)))
                    {
                        timer++;
                        incrementTime(taskList);
                        newProcess = shortestJob(taskList, timer); 
                    }
                    newProcess->progress++;
                    /*Create a new gantt entry if a new process is selected*/
                    if(newProcess != inProcess)
                    {
                        inProcess = newProcess;
                        newGantt->end = timer;/*put old task in gantt chart*/
                        insertLast(order, newGantt); 
                        /*Start new gantt*/
                        newGantt = (Gantt*)malloc(sizeof(Gantt));
                        newGantt->taskNo = inProcess->taskNo;
                        newGantt->start = timer;
                    }
     
                    timer++;
                    /*incrementTime(taskList);*/ 
                    /*Check if all tasks are complete*/
                    progressDone = isComplete(taskList); 
                  
                 }
                    if(progressDone)/*put last process in gantt*/
                    {
                        newGantt->end = timer;
                        insertLast(order, newGantt); 
                    }
                    /*printLinkedList(taskList, printPtr);*/
                    printf("average turnaround time = %f\n", aveTurnTime(taskList));
                    printf("average waiting time = %f\n", aveWaitTime(taskList));
                    printf("gantt: ");
                    printLinkedList(order, ganttPrint);
                    printf("|\n");
                    /*free*/
                    freeLinkedList(taskList, &free);
                    freeLinkedList(order, &free);      
            }
            else/*If no valid lines in file*/
            {
                freeLinkedList(order, &free);
                freeLinkedList(taskList, &free);
                printf("Error: Invalid file\n");
            }
            fclose(f);
        }
        
    }
            
    free(name);
    return 0;
}



















