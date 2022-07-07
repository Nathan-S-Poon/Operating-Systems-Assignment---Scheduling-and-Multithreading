/*
*PP and SRTF schedulers
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "scheduler.h"
#include "LinkedList.h"
#include "taskStruct.h"
#include "taskFunc.h"
#include "bufferStruct.h"

void *ppScheduler(void *vargp)
{
    FILE* f;
    char* name = (char*)malloc(10*sizeof(char*));
    char* output = (char*)malloc(100*sizeof(char)); 
    Task* task;
    Buffer* buffer;
    LinkedList* taskList;
    LinkedList* order;
    int priority, arrive, burst, taskNo, progressDone, timer, repeat;
    Task* inProcess;
    Task* newProcess;
    Gantt* newGantt;
    listFunc ganttPrint;
    inProcess = NULL;
    ganttPrint = &printGantt;
    repeat = TRUE;
    while(repeat)
    {
        buffer = (Buffer*)vargp;
        /*Conditional: wait for filename to be non empty*/
        pthread_mutex_lock(&buffer->inputMutex);
        while(strcmp(buffer->buffer1, "") == 0) 
        {
            buffer->fileWait++;/*Indicate that thread is waiting*/ 
            pthread_cond_wait(&buffer->inputCond, &buffer->inputMutex);
        }
        strcpy(name, buffer->buffer1);/*get filename from buffer*/ 
        buffer->haveRead++;
        /*If last process to cpoy then reset buffer*/
        if(buffer->haveRead == 2)
        {
            buffer->haveRead = 0;
            strcpy(buffer->buffer1, "");
            buffer->fileWait = 0;
        }
        pthread_mutex_unlock(&buffer->inputMutex);

        f = fopen(name, "r"); 
        if(strcmp(name, "QUIT") == 0)
        {
            repeat = FALSE;
        }
        else if((f == NULL))
        {
            strcpy(output, "Error: file is null\n");
        }
        else
        {
            /*Scheduling code*/
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
                   
            }/*simulate Priority scheduler*/
            timer = 0;
            progressDone = FALSE;
            inProcess = NULL;
            order = createLinkedList();/*for keeping track of process order*/

            if(listLength(taskList) != 0)
            {

                if(inProcess == NULL)/*For the start of the process*/
                {   /*Get first process*/
                    inProcess = highestPriority(taskList, timer);
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
                        inProcess = highestPriority(taskList, timer);
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
                    newProcess = highestPriority(taskList, timer);
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
                        newProcess = highestPriority(taskList, timer);
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
                           
                 
                /*print gantt*/
                pthread_mutex_lock(&buffer->printMutex);
                printf("Preemptive priority:\n");
                printf("gantt: ");
                printLinkedList(order, ganttPrint);
                printf("|\n");
                pthread_mutex_unlock(&buffer->printMutex);
 
                /*format output*/
                char part1[36] = "PP: the average turnaround time =";
                char part2[27] = "the average waiting time =";
                double turnTime = aveTurnTime(taskList);
                double waitTime = aveWaitTime(taskList);
                sprintf(output, "%s %.2f, %s %.2f\n", part1, turnTime, part2, waitTime);
                /*free*/
                fclose(f);
                freeLinkedList(taskList, &free);
                freeLinkedList(order, &free);     
            }
            else/*If not valid format output to be null*/
            {
                /*free*/
                freeLinkedList(taskList, &free);
                freeLinkedList(order, &free);     
                strcpy(output, "Error: Invalid file\n");
                fclose(f);
            }
                      
        }
        /*Send output to parent thread*/
        if(strcmp(name, "QUIT") != 0)
        {
            pthread_mutex_lock(&buffer->outputMutex);
            if(strcmp(buffer->buffer2, "") != 0)
            {
                pthread_cond_wait(&buffer->outputCond, &buffer->outputMutex);
            }
            strcpy(buffer->buffer2, output);             
            pthread_cond_signal(&buffer->outputCond);
            pthread_mutex_unlock(&buffer->outputMutex);         
        }
 

        
    }    
    free(output);
    free(name);
    pthread_mutex_lock(&buffer->printMutex);
    printf("PP terminate\n");
    pthread_mutex_unlock(&buffer->printMutex);

    return NULL;
} 
 
void *SRTFScheduler(void *vargp)
{
    FILE* f;
    Buffer* buffer;
    char* name = (char*)malloc(10*sizeof(char));
    char* output = (char*)malloc(100*sizeof(char));
    Task* task;
    LinkedList* taskList;
    LinkedList* order;
    int repeat, priority, arrive, burst, taskNo, progressDone, timer;
    Task* inProcess;
    Task* newProcess;
    Gantt* newGantt;
    listFunc ganttPrint;
    inProcess = NULL;
    repeat = TRUE;
    while(repeat)
    {
        buffer = (Buffer*)vargp;
        /*Conditional: wait for filename to be non empty*/
        pthread_mutex_lock(&buffer->inputMutex);
        while(strcmp(buffer->buffer1, "") == 0) 
        {
            buffer->fileWait++;/*Indicate that thread is waiting*/ 
            pthread_cond_wait(&buffer->inputCond, &buffer->inputMutex);
        }
        strcpy(name, buffer->buffer1);/*get filename from buffer*/ 
        buffer->haveRead++;
        /*If last process to cpoy then reset buffer*/
        if(buffer->haveRead == 2)
        {
            buffer->haveRead = 0;
            strcpy(buffer->buffer1, "");
            buffer->fileWait = 0;
        }      
        pthread_mutex_unlock(&buffer->inputMutex);
    
        f = fopen(name, "r"); 
        if(strcmp(name, "QUIT") == 0)
        {
            repeat = FALSE;
        }
        else if((f == NULL))
        {
            strcpy(output, "Error: file is null\n");
        }
        else
        {
            ganttPrint = &printGantt;
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
                   
            }/*simulate Priority scheduler*/
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
            
                /*print gantt- need to synchronise gantt*/
                pthread_mutex_lock(&buffer->printMutex);
                printf("SRTF:\n");
                printf("gantt: ");
                printLinkedList(order, ganttPrint);
                printf("|\n");
                pthread_mutex_unlock(&buffer->printMutex);
        
                /*format output*/
                char part1[36] = "SRTF: the average turnaround time =";
                char part2[27] = "the average waiting time =";
                double turnTime = aveTurnTime(taskList);
                double waitTime = aveWaitTime(taskList);
                sprintf(output, "%s %.2f, %s %.2f\n", part1, turnTime, part2, waitTime);
                fclose(f);
                freeLinkedList(taskList, &free);
                freeLinkedList(order, &free);   
            }
            else/*If not valid format output to be null*/
            {
                /*free*/
                fclose(f);
                freeLinkedList(taskList, &free);
                freeLinkedList(order, &free);     
                strcpy(output, "Error: Invalid file\n");
            }
  
          
        }
        /*copy to buffer*/
        if(strcmp(name, "QUIT") != 0)
        {
            pthread_mutex_lock(&buffer->outputMutex);
            if(strcmp(buffer->buffer2, "") != 0)
            {
                pthread_cond_wait(&buffer->outputCond, &buffer->outputMutex);
            }
            strcpy(buffer->buffer2, output);             
            pthread_cond_signal(&buffer->outputCond);
            pthread_mutex_unlock(&buffer->outputMutex);
        }        

    }
    free(output);        
    free(name);
    pthread_mutex_lock(&buffer->printMutex);
    printf("SRTF terminate\n");
    pthread_mutex_unlock(&buffer->printMutex);
 
    return NULL;
}
    



































































 
