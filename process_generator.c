#include "headers.h"
#include <stdio.h>
#include <string.h>
struct process proc;
struct PCB pcb;
struct pqueue procQ;
int msgq_id;
struct msgbuff
{
    long mtype;
    struct process P;
};
void clearResources(int);
void readfile(int algo){
    char *line=NULL;
    int size=0;
    int c=-1;
    char *values=malloc(size);
    FILE* file= fopen("prcss.txt","r");
    if (!file)
    {
        printf("Error opening file \n");
        return EXIT_FAILURE;
    }
    size_t linelength=0;
    ssize_t r;
    while (r=getline(&line,&linelength, file)!=-1) {
        if(line[0]!='#'){
            int n=0;
            int l=0;
            printf(line);
            while(n!=sizeof(line)+10){
                if(isdigit(line[n])){
                    int number=strtol(line,&line,10);
                    if(l==0){
                        //printf("%d number\n",number);
                        proc.id=number;
                        l++;
                    }
                    else if(l==1){
                        //printf("%d number\n",number);
                        pcb.arrivalTime=number;
                        l++;
                    }
                    else if(l==2){
                        //printf("%d number\n",number);
                        pcb.executionTime=number;
                        pcb.remainingTime=pcb.executionTime;
                        l++;
                        number=strtol(line,&line,10);
                        //printf("%d number\n",number);
                        proc.piriority=number;
                    }
                    else { 
                        printf("%d number\n",number);
                        l=0;
                    }
                    
                }
                else{
                    n++;
                    
                }
	        }
            pcb.responceTime=-1;
            pcb.turnArroundTime=-1;
            pcb.waitingTime=-1;
            pcb.weightedTurnArroundTime=-1;
            if(algo==0){
                pcb.pState='0';
                proc.saved=&pcb;
                enqueue(&procQ,&proc,proc.piriority);
            }
            else if(algo==1){
                pcb.pState='1';
                proc.saved=&pcb;
                enqueue(&procQ,&proc,proc.saved->remainingTime);
            }
            else{
                pcb.pState='2';
                proc.saved=&pcb;
                enqueue(&procQ,&proc,proc.saved->arrivalTime);
            }
            
            
        }
    }
    
    fclose(file);
}

int main()
{
    // TODO Initialization
    // 1. Read the input files.
    signal(SIGINT, clearResources);
    int key_id = ftok("keyfile", 65);
    msgq_id = msgget(key_id, 0666 | IPC_CREAT);
    //char const * const filename=argv[0];
    
    
    int sched,RRQ,send_id;
    
    printf("select schedular 0->(HPF),1->(SRTN),2->(RR):");
    scanf("%d",&sched);
    if(sched==2){
        printf("Enter the quantum of the Round Robin:");
        scanf("%d",&RRQ);
    }
    initialize(&procQ);
    readfile(sched);
    // 3. Initiate and create the scheduler and clock processes.
    char* argvs[]={"clk","c",NULL};
    char* args[]={"scheduler","c",NULL};
    int ex=execl("./clk.o",argvs);
    int exsched=execl("./scheduler.o",args);
    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this
    int x = getClk();
    printf("current time is %d\n", x);
    struct process temp;
    while(empty(&procQ)!=1){
        int x = getClk();
        printf("current time is %d\n", x);
        if(x==peek(&procQ)){
            struct msgbuff message;
            message.P=*dequeue(&procQ);
            //message.mtype = 100; //////checkkkk
            send_id = msgsnd(msgq_id, &message, sizeof(message), !IPC_NOWAIT);
            if (send_id == -1)
                perror("Errror in send\n");
        }
    }
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    
    msgctl(msgq_id, IPC_RMID, (struct msqid_ds *) 0);
    destroyClk(true);
    exit(0);
    //TODO Clears all resources in case of interruption
}