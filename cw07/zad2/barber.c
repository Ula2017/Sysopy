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
#include <sys/sem.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>




typedef struct waitClient{
    int max;
    pid_t pid[300];
    }waitClient;

waitClient *waiting = NULL;
int counter=0;


int shMem ;
sem_t* isSleep;
sem_t* chair;
sem_t* client;


void finish(){
//usuwamy semafor
    if(sem_close(isSleep) == -1 ){
        printf("Error during close semaphore number 0.\n");
    }
    if(sem_unlink("barber") == -1 ){
        printf("Error during delete semaphore number 0.\n");
    }
    if(sem_close(chair) == -1 ){
        printf("Error during close semaphore number 1.\n");
    }
    if(sem_unlink("chair") == -1 ){
        printf("Error during delete semaphore number 1.\n");
    }
    if(sem_close(client) == -1 ){
        printf("Error during close semaphore number 2.\n");
    }
    if(sem_unlink("clients") == -1 ){
        printf("Error during delete semaphore number 2.\n");
    }

    if(munmap(waiting, sizeof(waiting)) ){
        printf("Error during detache the shared memory segment.\n");
    }
    if(shm_unlink("sharedMem") ==-1){ // nazwa segmentu w argumencie
        printf("Error during delete the shared memory segment.\n");
    }

}

void createSemaphore(){


    isSleep = sem_open("barber", O_CREAT | O_EXCL | O_RDWR, 0666, 0);
    if(isSleep == SEM_FAILED ){
        printf("Error during create Semaphores.\n");
        exit(1);
    }

    chair = sem_open("chair", O_CREAT | O_EXCL | O_RDWR, 0666, 1);
    if(chair == SEM_FAILED ){
        printf("Error during create Semaphores.\n");
        exit(1);
    }

    client= sem_open("clients", O_CREAT | O_EXCL | O_RDWR, 0666, 1);
    if(client == SEM_FAILED ){
        printf("Error during create Semaphores.\n");
        exit(1);
    }

}

void initializeShMem(int number){


    //jaki ma byc rozmair pamieci dzielonej????
    //const char path[] = "/sharedMem";
    shMem = shm_open("sharedMem", O_CREAT | O_EXCL | O_RDWR, 0666 );
    if(shMem == -1 ){
        printf("Failed to create shared memory.\n");
        exit(1);
    }

    if(ftruncate(shMem, sizeof(waiting)) == -1){
        printf("Failed to truncate shared memory segment.\n");
        exit(1);
    }
    void * p=mmap(NULL, sizeof(waiting)*1000, PROT_READ | PROT_WRITE, MAP_SHARED, shMem, 0);
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

        struct timespec timer;

        if(sem_wait(isSleep) == -1){
            printf("Error during barber sleeping.\n");
            exit(1);
        }

        while(1){
        //zajmuje semafor client

        if(sem_wait(client) == -1){
            printf("Error during take client semaphore.\n");
            exit(1);
        }

        pid_t acCln = getClientToCut();

         //pobralem klienta wiec zwalniam semafor klientow
        if(sem_post(client) == -1){
            printf("Error during release client semaphore.\n");
            exit(1);
        }

        if(acCln == 0){

            //golibroda idzie spac
            clock_gettime(CLOCK_MONOTONIC, &timer);
            printf("Barber is going to sleep with time %ld\n.", (timer.tv_nsec/1000));
            fflush(stdout);

            if(sem_wait(isSleep) == -1){
                printf("Error during barber sleeping.\n");
                exit(1);
            }

        }else{

                if(sem_wait(chair)  == -1){
                printf("Error during take seat semaphore.\n");
                exit(1);
                }

                cut(acCln);

                //zwalniamy krzeslo
                if(sem_post(chair) == -1){
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

    return 0;

}
