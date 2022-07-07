/*
*To try thread stuff
*
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
 
     int number = 5;
     pthread_mutex_t lock;
void *myThreadFun(void *vargp)
{
   /* pthread_mutex_lock(&lock);*/
    printf("hello\n");
    number = number + *((int*)vargp);
    printf("%d\n", number); 
    
   /* pthread_mutex_unlock(&lock);*/
    return NULL;
}

int main()
{ 
    pthread_t thread_id2;    
    pthread_t thread_id;
    int* num;
    int* numtwo;
    num = (int*)malloc(sizeof(int));
    numtwo = (int*)malloc(sizeof(int));
    pthread_mutex_init(&lock, NULL);
    *num = 1;
    *numtwo = -1; 
    pthread_create(&thread_id, NULL, myThreadFun, (void*)numtwo);
    pthread_create(&thread_id2, NULL, myThreadFun, (void*)num);
   
    pthread_join(thread_id, NULL); 
    pthread_join(thread_id2, NULL);
    exit(0);
}


















