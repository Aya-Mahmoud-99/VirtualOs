#include "headers.h"
static void finishHandler(int signum);

/*struct process{
int id;
int piriority;
struct PCB* saved;
};*/

/*struct process* dequeue(struct process** queue){
//
}*/
struct msgbuff
{
    long mtype;
    //char mtext[70];
    struct process* p;
};
pqueue* q;
int * timeStep;
struct process* currentProcess; //current running process
FILE* fout;
FILE* statistics;
struct msgbuff received_message;
float avgWTA;
float avgWAITING;
int noProcesses;
float* WTAS; //array that saves weighting arround times to use it to calculate standard deviation
float stdWTA;
float utilization;

float calculateSD(float data[]) {
    float sum = 0.0, mean, SD = 0.0;
    int i;
    for (i = 0; i < 10; ++i) {
        sum += data[i];
    }
    mean = sum / 10;
    for (i = 0; i < 10; ++i)
        SD += pow(data[i] - mean, 2);
    return sqrt(SD / 10);
}

void hpfSchedule(){
  signal(SIGCONT,finishHandler);// THE SIGNAL THE PROCESS CALLS WHEN IT FINISHE
  utilization=100;
  noProcesses=0;
  avgWTA=0;
  fout=fopen("scheduler.log","w");
  fout=fopen("scheduler.log","w");
  fprintf(fout,"#At\ttime\tx\tprocess\ty\tstate\tarr\tw\ttotal\tz\tremain\ty\twait\tk");
  // get the shared varible with the clock to get the current time
  int shmid = shmget(SHKEY, 4, IPC_CREAT | 0644);
  timeStep= (int *) shmat(shmid, (void *)0, 0);
  int key_id = ftok("keyfile", 65);
  WTAS=malloc(sizeof(float));
  int msgq_id = msgget(key_id, 0666 | IPC_CREAT);
  int rec_val;
  while(rec_val = msgrcv(msgq_id, &received_message, sizeof(received_message.p), 0,     IPC_NOWAIT)!=-1){
      //call enqueue inpiriority queue
     //q.enqueue(received_message.p);
     enqueue(q,received_message.p,received_message.p->piriority);
    }
  while(currentProcess=dequeue(q)){
    noProcesses++;
    WTAS=realloc(WTAS,noProcesses*sizeof(float));
    //call dequeue from piriority queue
    
    currentProcess->saved->responceTime=*timeStep-currentProcess->saved->arrivalTime;
    currentProcess->saved->waitingTime=*timeStep-currentProcess->saved->arrivalTime;
    currentProcess->saved->pState=RUNNING;
    fprintf(fout,"At\ttime\t%d\tprocess\t%d\tstarted\tarr\t%d\ttotal\t%d\tremain\t%d\twait\t%d\n",*timeStep,currentProcess->id,currentProcess->saved->arrivalTime,currentProcess->saved->executionTime,currentProcess->saved->executionTime,currentProcess->saved->waitingTime);
    int id=fork();
    // if it is child change its code with the code of the process file and send it the remainig time parameter equals execution time
    //change process.c with the full path
    if(id==0)execl("process.c","process.bin",currentProcess->saved->executionTime,NULL);
    //else p->id=id;
    //pause untill the process awakens it after it finishes execution
    pause();
    currentProcess->saved->turnArroundTime=*timeStep-currentProcess->saved->arrivalTime;
    currentProcess->saved->weightedTurnArroundTime=currentProcess->saved->turnArroundTime/currentProcess->saved->executionTime;
    WTAS[noProcesses-1]=currentProcess->saved->weightedTurnArroundTime;
    avgWTA=avgWTA+currentProcess->saved->weightedTurnArroundTime;
    avgWAITING=avgWAITING+currentProcess->saved->waitingTime;
    
    while(rec_val = msgrcv(msgq_id, &received_message, sizeof(received_message.p), 0,     IPC_NOWAIT)!=-1){
      //call enqueue inpiriority queue
     //q.enqueue(received_message.p);
     enqueue(q,received_message.p,received_message.p->piriority);
    }
  }
  avgWAITING=avgWAITING/noProcesses;
  avgWTA=avgWTA/noProcesses;
  stdWTA=calculateSD(WTAS);
  fprintf(statistics,"CPU\tutilization\t=\t%f%%",utilization);
  fprintf(statistics,"Avg\tWTA\t=\t%f\n",avgWTA);
  fprintf(statistics,"Avg\tWaiting\t=\t%f\n",avgWAITING);
  fprintf(statistics,"Std\tWTA\t=\t%f\n",stdWTA);
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
void finishHandler(int signum){
    fprintf(fout,"At\ttime\t%d\tprocess\t%d\tfinished\tarr\t%d\ttotal\t%d\tremain\t%d\twait\t%d\tTA\t%d\tWTA\t%d\n",*timeStep,currentProcess->id,currentProcess->saved->arrivalTime,currentProcess->saved->executionTime,currentProcess->saved->executionTime,currentProcess->saved->waitingTime,currentProcess->saved->turnArroundTime,currentProcess->saved->weightedTurnArroundTime);
  //clean process resources
}
