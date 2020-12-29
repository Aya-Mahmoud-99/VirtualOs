#include "headers.h"
//flag sleeping ;sleeping is set true before a process sleeps and false when it wakes up
/*
handlers //in case of shortest job first
if a certain signal is sent from the process generator put it in the queue according to its parameter
// if a process is running
stop the running process
enqueue it again



///in case a process has finished 
*/
/*
enqueue
*/
struct PCB{
proState pState;
int exectionTime;
int remainingTime;
};
struct process{
int id;
struct PCB* saved;
};
struct process* dequeue(struct process** queue){
//
}
struct msgbuff
{
    long mtype;
    //char mtext[70];
    struct process p;
};
struct process* running;
int main(int argc, char * argv[]){
    initClk();
    schedTech technique=atoi(argv[1]);
    int quantum=atoi(argv[1]);
    key_t key_id;
    key_id = ftok("keyfile", 70);               //create unique key
    int runningProcessState = shmget(key_id, 4096, IPC_CREAT | 0644); //reserve shared
    key_id = ftok("keyfile", 65);
    int msgq_id = msgget(key_id, 0666 | IPC_CREAT); //queue to send message
    
    struct process* queue[100];
    while(1){
    struct process* p=dequeue(queue);
    running=p;
    //if the process.id==-1(hasn't started before) then fork it and set its id
    if(p->id==-1){
    int id=fork();
       if(id<0){
    }
    else if(id==0){
      //execel("process.o","process.o",""); give it the parameters
    }
    else{
    p->id=id;
    }
    }
    else{
    signal(SIGCONT,p->id);
    }
    // if the process.id not equal -1 send a signal to the this process to resume it
    // if round robin sleeps for a quantum then enqueu it again if it hasn't finished
    void *currPCR = shmat(runningProcessState, (void *)0, 0);
    struct msgbuff received_message;
    switch(technique){
    case RR:
    sleep(quantum);
    signal(SIGSTOP,p->id);
    p->saved=(struct PCB*)currPCR;
    if(p->saved->remainingTime!=0) roundEnqueu(p);
    break;
    case HPF:
    //if highest piriority first (non premptive) sleep until the process send a signal it has finished
    pause();
    p->saved=(struct PCB*)currPCR;
    while(msgrcv(msgq_id, &received_message,    sizeof(received_message.p), 0,     IPC_NOWAIT)!=-1){
      hpfEnqueue(received_message.p,queue);
    }
    break;
    case STRN:
    //if shortest job first sleep until process awakens it  when it finishes or it awakens itself in handler if aprocess is put in the first queue and if it hasn't finished enqueue it again
    pause();
    p->saved=p->saved=(struct PCB*)currPCR;
    if(p->saved->remainingTime!=0) strnEnqueu(p,queue);
    break;
         // if highest piriority queue or round robin receive the last messages of the entered processes and queue them
    }
    }
    
    //TODO implement the scheduler :)
    //upon termination release the clock resources.
    
    destroyClk(true);
}
