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


typedef struct waitClient{
    int max;
    pid_t pid[300];
    }waitClient;

int sem;
waitClient *waiting = NULL;
int counter=0;
void* p;
int shMem ;


void finish(){
//usuwamy semafor
    if(semctl(sem, 0, IPC_RMID) == -1 ){
        printf("Error during delete semaphore.\n");
    }
    if(shmdt(p) == -1 ){
        printf("Error during detache the shared memory segment.\n");
    }
    if(shmctl(shMem, IPC_RMID, NULL) ==-1){
        printf("Error during delete the shared memory segment.\n");
    }

}

void createSemaphore(){


    key_t key = ftok(getenv("HOME"), 2);
    sem = semget(key,3, IPC_CREAT | 0666 );
    if(sem == -1 ){
        printf("Error during create Semaphores.\n");
        exit(1);
    }
    //0 - czy golibroda spi, 1- czy krzeslo do strzyzenia zajete 2 - czy poczekalnia pelna
    if(semctl(sem, 0, SETVAL, 0) == -1 || semctl(sem, 1, SETVAL, 1) == -1 || semctl(sem, 2, SETVAL, 1) == -1 ){
        printf("Error during set value.\n");
        exit(1);
    }

}

void initializeShMem(int number){

    key_t key = ftok(getenv("HOME"), 7);
    if(key ==-1){
        printf("Failed to create fifo key\n");
        exit(1);
    }
    //jaki ma byc rozmair pamieci dzielonej????
    shMem = shmget(key, sizeof(pid_t)*100, IPC_CREAT | 0666);
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
    waiting->max=number;
    memset(waiting->pid, 0, 300);


}
pid_t getClientToCut(){

    if(waiting->pid[0] == 0) return 0; //nie ma klientow na poczekalni

    pid_t toReturn = waiting->pid[0];
    for(int i=0; i<299; i++) waiting->pid[i] = waiting->pid[i+1];

    return toReturn;

}

void cut(pid_t client){

    struct timespec timer;
    clock_gettime(CLOCK_MONOTONIC, &timer);
    printf("Barber is going to cut client which pid is %d with time %ld.\n",(int)client, (timer.tv_nsec/1000));
    fflush(stdout);
    kill(client,SIGUSR1);
    clock_gettime(CLOCK_MONOTONIC, &timer);
    printf("Barber finish the cut for client which pid is %d with time %ld. \n",(int)client, (timer.tv_nsec/1000));
    fflush(stdout);
}


void work(){

        struct sembuf buffor;
        buffor.sem_num=0;
       buffor.sem_op = -1; //semafor jest ustawiony na 1 i czekamy az ktos go zmniejszy
        buffor.sem_flg =0;

        struct timespec timer;

        if(semop(sem, &buffor, 1) == -1){
            printf("Error during barber sleeping.\n");
            exit(1);
        }

        while(1){

        buffor.sem_num=2;
        buffor.sem_op=-1; //zajmuje semafor client

        if(semop(sem, &buffor, 1) == -1){
            printf("Error during take client semaphore.\n");
            exit(1);
        }
        pid_t acCln = getClientToCut();

        buffor.sem_num=2;
        buffor.sem_op=1; //pobralem klienta wiec zwalniam semafor klientow
        if(semop(sem, &buffor, 1) == -1){
            printf("Error during take client semaphore.\n");
            exit(1);
        }

        if(acCln == 0){

            //golibroda idzie spac
            buffor.sem_num=0;
            buffor.sem_op = -1;

            clock_gettime(CLOCK_MONOTONIC, &timer);
            fflush(stdout);
            printf("Barber is going to sleep with time %ld.\n", (timer.tv_nsec/1000));
            fflush(stdout);

            if(semop(sem, &buffor, 1) == -1){
                printf("Error during barber sleeping.\n");
                exit(1);
            }

        }else{
                buffor.sem_num=1;
                buffor.sem_op=-1; //1 semafor krzesło zajety
                if(semop(sem, &buffor, 1) == -1){
                printf("Error during take seat semaphore.\n");
                exit(1);
                }

                cut(acCln);

                buffor.sem_num=1;
                buffor.sem_op=1; //zwalniamy krzeslo
                if(semop(sem, &buffor, 1) == -1){
                printf("Error during untake seat semaphore.\n");
                exit(1);
                }
            }
        }
}

int main(int argc, char** argv){


    atexit(finish);
    signal(SIGINT,SIG_DFL );

    if(argc !=2){
        printf("Wrong number of arguments. Usage: ./barber chairsNumber.\n");
        exit(1);
    }

    int chairsWait = atoi(argv[1]);
    if(chairsWait <=0 ){
        printf("Wrong second argument. Second argument should be number.\n");
        exit(1);
    }


    createSemaphore();
    initializeShMem(chairsWait);

    work();

}
