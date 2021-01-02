#include "headers.h"
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
pQueue q;
struct msgbuff received_message;
void hpfSchedule(){
  signal(SIGCONT,handler);// THE SIGNAL THE PROCESS CALLS WHEN IT FINISHES
  // get the shared varible with the clock to get the current time
  int shmid = shmget(SHKEY, 4, IPC_CREAT | 0644);
  int * timeStep = (int *) shmat(shmid, (void *)0, 0);
  int key_id = ftok("keyfile", 65);
  int msgq_id = msgget(key_id, 0666 | IPC_CREAT);
  while(1){
    struct process* p=q.dequeue();
    //call dequeue from piriority queue
    p->saved->responceTime=*timeStep-p->saved->arrivalTime;
    p->saved->waitingTime=*timeStep-p->saved->arrivalTime;
    p->saved->pState=RUNNING;
    int id=fork();
    // if it is child change its code with the code of the process file and send it the remainig time parameter equals execution time
    //change process.c with the full path
    if(id==0)execl("process.c","process.bin",p->saved->executionTime);
    else p->id=id;
    p->saved->turnArroundTime=*timeStep-p->saved->arrivalTime;
    p->saved->weightedTurnArroundTime=p->saved->turnArroundTime/p->saved->executionTime;
    //pause untill the process awakens it after it finishes execution
    pause();
    int rec_val = msgrcv(msgq_id, &received_message, sizeof(received_message.p), 0,     IPC_NOWAIT);
    if(rec_val!=-1){
      //call enqueue inpiriority queue
     q.enqueue(p);
    }
    
  }
}
int main(int argc, char * argv[]){
    initClk();
    schedTech technique=atoi(argv[1]);
    switch(technique){
      case HPF:
      hpfSchedule();
      break;

    }
    // after finishing report ang ouput the file
    //TODO implement the scheduler :)
    //upon termination release the clock resources.
    
    destroyClk(true);
}
void handler(){
  //clean process resources
}
