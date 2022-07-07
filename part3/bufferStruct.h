/*
*Contains struct sending trhough buffer
*fileWait - int for how many threads are waiting for input
*inputCond - condition for reading filename
*inputMutex - mutex that protects while(buffer is empty) 
*printMutex - protects printing
*/

#ifndef BUFFER
#define BUFFER

typedef struct Buffer
{
    char* buffer1;
    int fileWait;
    pthread_cond_t inputCond;
    pthread_mutex_t inputMutex;
    pthread_mutex_t printMutex;
    char* buffer2;
    pthread_cond_t outputCond;
    pthread_mutex_t outputMutex;
    int haveRead;
    int terminate;
    pthread_cond_t termCond;
    pthread_mutex_t termMutex;
    
} Buffer;


#endif
