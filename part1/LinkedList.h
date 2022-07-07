#ifndef LINKEDLIST
#define LINKEDLIST
typedef struct ListNode
{
    void* data;
    struct ListNode* next;
    struct ListNode* prev;
} ListNode;

typedef struct LinkedList
{
    int count;
    ListNode* head;
    ListNode* tail;
} LinkedList;

typedef void (*listFunc)(void* data);

LinkedList* createLinkedList();
void* removeStart(LinkedList* list);
void insertStart(LinkedList* list, void* entry);
void insertLast(LinkedList* list, void* entry);
void* removeLast(LinkedList* list);
void printLinkedList(LinkedList* list, listFunc funcPtr);
void freeLinkedList(LinkedList* list, listFunc funcPtr);
int listLength(LinkedList* list);
#endif









