#define MAX 30
#include <stddef.h>
//Enum of process state
typedef enum{WAITING,RUNNING}proState;
struct PCB{
proState pState;
int executionTime;
int remainingTime;
int waitingTime;
int responceTime;
int arrivalTime;
int turnArroundTime;
int weightedTurnArroundTime;
};

struct process{
int id;
int piriority;
struct PCB* saved;
};
typedef struct pqueue
{
    struct process* data[MAX];
    int piriority[MAX];
    int rear,front;
}pqueue;
void initialize(pqueue *p);
int empty(pqueue *p);
int full(pqueue *p);
void enqueue(pqueue *p, struct process* pr,int x);
struct process* dequeue(pqueue *p);
void initialize(pqueue *p)
{
    p->rear=-1;
    p->front=-1;
}
 
int empty(pqueue *p)
{
    if(p->rear==-1)
        return(1);
 
    return(0);
}
 
int full(pqueue *p)
{
    if((p->rear+1)%MAX==p->front)
        return(1);
 
    return(0);
}
void enqueue(pqueue *p, struct process* pr,int x)
{
    int i;
    if(full(p))
        printf("\nOverflow");
    else
    {
        if(empty(p))
        {
            p->rear=p->front=0;
            p->data[0]=pr;
            p->piriority[0]=x;
        }
        else
        {
            i=p->rear;
 
            while(x>p->piriority[i])
            {
                p->data[(i+1)%MAX]=p->data[i];
                p->piriority[(i+1)%MAX]=p->piriority[i];
                i=(i-1+MAX)%MAX; //anticlockwise movement inside the queue
                if((i+1)%MAX==p->front)
                    break;
            }
 
            //insert x
            i=(i+1)%MAX;
            p->data[i]=pr;
            p->piriority[i]=x;
 
            //re-adjust rear
            p->rear=(p->rear+1)%MAX;
        }
    }
}
 
struct process* dequeue(pqueue *p)
{
    struct process* x;
 
    if(empty(p))
    {
        //printf("\nUnderflow..");
        return NULL;
    }
    else
    {
        x=p->data[p->front];
        if(p->rear==p->front)   //delete the last element
            initialize(p);
        else
            p->front=(p->front +1)%MAX;
    }
 
    return(x);
}
 