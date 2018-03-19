#include  <stdlib.h>
#include  <stdio.h>
#include  <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include<sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

typedef struct waitClient{
    int max;
    pid_t pid[300];
    }waitClient;

pid_t child;
int sem;
int shMem;
void* p;
waitClient *waiting;


int cut=0;

void finish(){
    if(shmdt(p) == -1) printf("Error during detache the shared memory segment.\n");
}

void handlerSIGUSR1(int x){

    cut++;
    printf("Process %d was cut %d times.\n", (int)child, cut);
    fflush(stdout);

}

void handlerSIGUSR2(int x){

    struct timespec timer;
    clock_gettime(CLOCK_MONOTONIC, &timer);
    printf("There is no seats. I left the barber unhappy with time %ld.\n", (timer.tv_nsec/1000));
    fflush(stdout);
    exit(1);

}

void createSemaphore(){


    key_t key = ftok(getenv("HOME"), 2);
    sem = semget(key,0,0);
    if(sem == -1 ){
        printf("Error during create Semaphores.\n");
        exit(1);
    }

}

void initializeShMem(){

    key_t key = ftok(getenv("HOME"), 7);
    if(key ==-1){
        printf("Failed to create fifo key\n");
        exit(1);
    }

    shMem = shmget(key, 0,0);
    if(shMem == -1 ){
        printf("Failed to create shared memory.\n");
        exit(1);
    }

    p=shmat(shMem,NULL,0);
    if(p == (void*)(-1) ){
        printf("Failed to attach the shared memory segment.\n");
        exit(1);
    }
    waiting = (waitClient*) p;
}

void addClient(){

    int i=0;

    while(waiting->pid[i] != 0){
        i++;
    }
    if(i == waiting ->max) kill(getpid(), SIGUSR2); //kolejka pelna czyli nie mozemy dodac - powinnismy sie zabijac
    waiting->pid[i]=getpid();


}


void goToBarber(){

   struct sembuf buffor;
    struct timespec timer;

    buffor.sem_flg =0;
    buffor.sem_num=2;
    buffor.sem_op=-1; //zajmuje semafor client

    if(semop(sem, &buffor, 1) == -1){
        printf("Error during take client semaphore.\n");
        exit(1);
    }

    addClient();

    buffor.sem_num=2;
    buffor.sem_op=1; //pobralem klienta wiec zwalniam semafor klientow

    if(semop(sem, &buffor, 1) == -1){
        printf("Error during take client semaphore.\n");
        exit(1);
    }

    int isSleep = semctl(sem, 0, GETVAL);

    if(isSleep == -1){
        printf("Error during check barber status.\n");
        exit(1);
    }

    if(isSleep == 0){
        buffor.sem_num=0;
        buffor.sem_op=1;

        clock_gettime(CLOCK_MONOTONIC, &timer);
        printf("Barber was awake by client %d with time %ld.\n",(int)getpid(), (timer.tv_nsec/1000));

        if(semop(sem, &buffor, 1) == -1){
            printf("Error during take seat semaphore.\n");
            exit(1);
        }
    }
    else{
        clock_gettime(CLOCK_MONOTONIC, &timer);
        printf("Client %d took a seat in the waiting room with time %ld.\n",(int)getpid(), (timer.tv_nsec/1000));
    }


}


int main(int argc, char** argv){

    atexit(finish);
    signal(SIGINT,SIG_DFL );

    if(argc !=3){
        printf("Wrong number of arguments. Usage: ./clients clientsNumber cutNumber.\n");
        exit(1);
    }

    int clientsNum = atoi(argv[1]);
    int cutsNum = atoi(argv[2]);
    if(clientsNum <=0 || cutsNum <= 0 ){
        printf("Wrong second or third argument. Second  and third argument should be number.\n");
        exit(1);
    }

    createSemaphore();
    initializeShMem();

    for(int j=0; j<clientsNum; j++){
        pid_t pid = fork();
        if(pid ==0 ){

            child=getpid();
            signal(SIGUSR1, handlerSIGUSR1);
            signal(SIGUSR2, handlerSIGUSR2);

            for(int i=0; i<cutsNum; i++){

                    goToBarber();
                    pause(); //czekamy na sygnał SIGUSR1
            }

            struct timespec timer;
            clock_gettime(CLOCK_MONOTONIC, &timer);
            printf("Client %d was cut %d times with time %ld.\n",(int)getpid(), cutsNum, (timer.tv_nsec/1000));
            fflush(stdout);

            return 0;
        }else if(pid <0 ){
            printf("Error during fork.\n");
            exit(1);
        }

        }
        pid_t wpid;
        int status;
        wpid = wait(&status);
            while (wpid > 0) wpid = wait(&status);



        return 0;

}
