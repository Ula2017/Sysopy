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
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

typedef struct waitClient{
    int max;
    pid_t pid[300];
    }waitClient;

pid_t child;
int shMem ;
sem_t* isSleep;
sem_t* chair;
sem_t* client;

waitClient *waiting;


int cut=0;

void finish(){
    if(sem_close(isSleep) == -1 ){
        printf("Error during close semaphore number 0.\n");
    }
    if(sem_close(chair) == -1 ){
        printf("Error during close semaphore number 1.\n");
    }
    if(sem_close(client) == -1 ){
        printf("Error during close semaphore number 2.\n");
    }

    if(munmap(waiting, sizeof(waiting)) ){
        printf("Error during detache the shared memory segment.\n");
    }

}

void handlerSIGUSR1(int x){

    cut++;

}

void handlerSIGUSR2(int x){

    struct timespec timer;
    clock_gettime(CLOCK_MONOTONIC, &timer);
    printf("There is no seats. I left the barber unhappy with time %ld.\n", (timer.tv_nsec/1000));
    exit(1);

}

void createSemaphore(){


    isSleep = sem_open("barber", O_RDWR);
    if(isSleep == SEM_FAILED ){
        printf("Error during create Semaphores.\n");
        exit(1);
    }

    chair = sem_open("chair", O_RDWR);
    if(chair == SEM_FAILED ){
        printf("Error during create Semaphores.\n");
        exit(1);
    }

    client= sem_open("clients",O_RDWR);
    if(client == SEM_FAILED ){
        printf("Error during create Semaphores.\n");
        exit(1);
    }
}

void initializeShMem(){

    shMem = shm_open("sharedMem", O_RDWR, 0666 );
    if(shMem == -1 ){
        printf("Failed to create shared memory.\n");
        exit(1);
    }

    void* p=mmap(NULL, sizeof(waiting), PROT_READ | PROT_WRITE, MAP_SHARED, shMem, 0);
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

    struct timespec timer;
    //zajmuje semafor client

    if(sem_wait(client) == -1){
        printf("Error during take client semaphore.\n");
        exit(1);
    }
    addClient();

     //pobralem klienta wiec zwalniam semafor klientow
    if(sem_post(client) == -1){
        printf("Error during take client semaphore.\n");
        exit(1);
    }

    int sleeping;
    sem_getvalue(isSleep, &sleeping);
    if(sleeping == -1){
        printf("Error during check barber status.\n");
        exit(1);
    }

    if(sleeping == 0){

        clock_gettime(CLOCK_MONOTONIC, &timer);
        printf("Barber was awake by client %d with time %ld.\n",(int)getpid(), (timer.tv_nsec/1000));
        fflush(stdout);

        if(sem_post(isSleep)== -1){
            printf("Error during take seat semaphore.\n");
            exit(1);
        }
    }
    else{
        clock_gettime(CLOCK_MONOTONIC, &timer);
        printf("Client %d took a seat in the waiting room with time %ld.\n",(int)getpid(), (timer.tv_nsec/1000));
        fflush(stdout);

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

    for(int i=0; i<clientsNum; i++){
        pid_t pid = fork();
        if(pid ==0 ){

            child=getpid();
            signal(SIGUSR1, handlerSIGUSR1);
            signal(SIGUSR2, handlerSIGUSR2);
            cut = 1;
            for(int j=0; j<cutsNum; j++){

                goToBarber();
                fflush(stdout);
                pause(); //czekamy na sygnał SIGUSR1
            }

            struct timespec timer;
            clock_gettime(CLOCK_MONOTONIC, &timer);
            printf("Client %d was cut %d times with time %ld. Finish.\n",(int)getpid(), cutsNum, (timer.tv_nsec/1000));
            fflush(stdout);

            return 0;
        }else if(pid <0){
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
