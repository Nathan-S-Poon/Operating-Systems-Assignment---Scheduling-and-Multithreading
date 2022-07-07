/* 
*Parent makes threads to call PP and SRTF
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "scheduler.h"
#include "bufferStruct.h"
 
Buffer* createBuffer()
{
    Buffer* buffer;
    buffer = (Buffer*)malloc(sizeof(Buffer));
    buffer->buffer1 = (char*)malloc(10*sizeof(char));
    strcpy(buffer->buffer1,"");
    buffer->buffer2 = (char*)malloc(100*sizeof(char));
    strcpy(buffer->buffer2,"");
    buffer->inputCond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    buffer->inputMutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    buffer->printMutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    buffer->outputCond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    buffer->outputMutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    buffer->fileWait = 0;
    buffer->haveRead = 0;
    return buffer;
}

void freeBuffer(Buffer* buffer)
{
    free(buffer->buffer1);
    free(buffer->buffer2);
    free(buffer);
}


char* outputBuffer(Buffer* buffer)
{
    char* result = (char*)malloc(100*sizeof(char));
    pthread_mutex_lock(&buffer->outputMutex);
    while(strcmp(buffer->buffer2, "") == 0)
    {
        pthread_cond_wait(&buffer->outputCond, &buffer->outputMutex);
    }
    strcpy(result, buffer->buffer2);
    strcpy(buffer->buffer2, "");
    pthread_cond_signal(&buffer->outputCond);/*release second schedule*/
    pthread_mutex_unlock(&buffer->outputMutex);
    return result;
}


int main()
{
    Buffer* buffer;
    char* name;
    char* out1;
    char* out2;
    pthread_t thread_A;
    pthread_t thread_B;
    buffer = createBuffer();
    name = (char*)malloc(10*sizeof(char));
       
    /*create p thread for PP*/
    pthread_create(&thread_A, NULL, ppScheduler, (void*)buffer);
    pthread_create(&thread_B, NULL, SRTFScheduler, (void*)buffer);
   
    do     
    {     
        printf("PP and SRTF simulation:\n");
        scanf("%s", name);   
        if(strcmp(name, "QUIT") != 0)
        {
            /*signal and let threads continue*/
            pthread_mutex_lock(&buffer->inputMutex);
            strcpy(buffer->buffer1, name);
            if(buffer->fileWait > 0)/*Check if anyone is waiting*/
            {
                pthread_cond_broadcast(&buffer->inputCond);
            }            
            pthread_mutex_unlock(&buffer->inputMutex);  
 
            /*Conditional: wait for input*/
            out1 = outputBuffer(buffer);
            /*for both inputs*/ 
            out2 = outputBuffer(buffer);
            /*mutual exclusion on printing*/
            pthread_mutex_lock(&buffer->printMutex);
            printf("\n%s%s", out1, out2);
            pthread_mutex_unlock(&buffer->printMutex);           
            free(out1);
            free(out2);
        }
        else if(strcmp(name, "QUIT") == 0)
        {
            pthread_mutex_lock(&buffer->inputMutex);
            strcpy(buffer->buffer1, name);
            if(buffer->fileWait > 0)/*Check if anyone is waiting*/
            {
                pthread_cond_broadcast(&buffer->inputCond);
            }           
            pthread_mutex_unlock(&buffer->inputMutex);  
        }

    }while(strcmp(name, "QUIT") != 0);

    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
    printf("Parent terminate\n");
    freeBuffer(buffer);
    free(name);
    return 0;
}








