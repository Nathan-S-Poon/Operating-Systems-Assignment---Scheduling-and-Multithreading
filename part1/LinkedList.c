/**********************************************************************
*LinkedList
*Description: Code needed to contsruct a linked list
*Submitted for UCP assignment on the 25/05/20
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

/****************************************************
 *starts a linked list by creating a pointer to a
 *LinkedList struct. Sets the field 'head' to null
 ***************************************************/
LinkedList* createLinkedList() 
{ 
    LinkedList* list; 
    list = (LinkedList*)malloc(sizeof(LinkedList));    
    (*list).head = NULL; 
    (*list).tail = NULL;
    (*list).count = 0;
    return list;
}

/**************************************************
 *Insert value at start of list.
 *input list pointer and new value to store 
 *************************************************/
void insertStart(LinkedList* list, void* entry)
{
    /*create new node*/
    ListNode* newNode;
    newNode = (ListNode*)malloc(sizeof(ListNode));
    /*newNode's data points to the entered value*/
    (*newNode).data = entry;     
    /*have newNode's head point to node that head was pointing to ie first node*/
    if((*list).head != NULL)/*If there are other nodes - set prev to newNode*/
    { 
        (*(*list).head).prev = newNode; 
        (*newNode).next = (*list).head;
        (*list).head = newNode;
   }
    else/*if first node to be inserted - set tail to first node*/
    {
        (*list).head = newNode;
        (*list).tail = newNode;
        (*newNode).next = NULL;
   }
    (*newNode).prev = NULL;
    /*increment count*/
    (*list).count++;
} 

/**************************************************
 *removeStart
 *make a copy of head pointer and set head to point
 *to second element (first nodes next). 
 *Free temp and temp's value
 *************************************************/
void* removeStart(LinkedList* list)
{
    ListNode* temp;
    void* tempData;/*store data that will be delete for return*/
    tempData = NULL;
    temp = (*list).head;
    if(temp != NULL)
    {
        tempData = (*temp).data;   
        (*list).head = (*((*list).head)).next;/*head points to second node*/
        if((*list).head == NULL)/*if last one was removed*/
        {
            (*list).tail = NULL; 
        }
        else/*otherwise make new first node's prev = null*/
        {
           (*(*list).head).prev = NULL;
        }    
    
        /*If delete value*/ 
        /*free((*temp).data);*/
        free(temp);
    
        /*decrement count*/
        (*list).count--;
    }
    return tempData;
} 


/**************************************************
 *insertLast
 *similar to insert first but adding to tail
 *************************************************/
void insertLast(LinkedList* list, void* entry)
{
    ListNode* newNode;
    newNode = (ListNode*)malloc(sizeof(ListNode));
    (*newNode).data = entry;
    
    /*If list is empty set head to newNode*/
    if((*list).head == NULL) 
    {
        (*list).head = newNode;
        (*(*list).head).prev = NULL; 
        newNode->next = NULL;
    }
    else/*set newNode's prev to last node*/
    {
        (*(*list).tail).next = newNode;
        (*newNode).prev = (*list).tail; 
        (*newNode).next = NULL;
    }
    /*tail point to new node*/
    (*list).tail = newNode;
    /*increment count*/
    (*list).count++;
} 

/**************************************************
 *removeLast
 *similar to removeStart but from the tail
 *************************************************/
void* removeLast(LinkedList* list)
{
   /*create temp as a copy of deleted node*/
    ListNode* temp;
    void* tempData;
    tempData = NULL;
    temp = (*list).tail;
    if(temp != NULL)
    {
        tempData = (*temp).data;
        (*list).tail = (*((*list).tail)).prev;/*set tail to lastnode's prev*/    
        if((*list).tail == NULL)/*if last one was removed*/
        {
            (*list).head = NULL; 
        }
        else
        {
            (*(*list).tail).next = NULL;
        }
        /*Free*/
        /*free((*temp).data);*/
        free(temp); 
        /*decrement count*/
        (*list).count--;   
    }
    return tempData;
} 

/**************************************************
 *printLinkedList
 *input pointer to function that outputs more complex
 *data e.g. array.
 *************************************************/
void printLinkedList(LinkedList* list, listFunc funcPtr)
{
    ListNode *node;
    node = (*list).head;
    while(node != NULL)
    {
        (*funcPtr)((*node).data);/*print*/
        node = (*node).next;/*get next in list*/
    }
    
}         


/**************************************************
 *freeLinkedList
 *input pointer to function that frees data for more
 *complex data. Other wise input &free
 *************************************************/
void freeLinkedList(LinkedList* list, listFunc funcPtr)
{
    ListNode *node, *nextNode;
    node = NULL;
    node = (*list).head;/*set node to first node*/
    while(node != NULL)
    {
        nextNode = (*node).next;/*set ref to next node before delete*/
        (*funcPtr)((*node).data);/*free data*/
        free(node); 
        node = nextNode;
    }
    free(list);

} 

/*********************************************************
 *listLength
 ************************************************************/
int listLength(LinkedList* list)
{
    int length;
    length = (*list).count;
    return length;
}






