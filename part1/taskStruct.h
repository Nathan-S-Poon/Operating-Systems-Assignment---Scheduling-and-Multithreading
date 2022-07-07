/*
*contains task struct where task info is stored
*e. arrival time and burst size
*/

typedef struct Task
{
    int taskNo;
    int arrival;
    int burst;
    int priority;/*lower means higher priority*/
    int turn;
    int progress;
} Task;

/*
*keeps track of process in CPU and its start/end time
*
*/
typedef struct Gantt
{
    int taskNo;
    int start;
    int end;
} Gantt;
