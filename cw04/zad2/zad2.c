#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<signal.h>
#include <sys/types.h>
#include<string.h>
#include <time.h>
//0-nazwa programu, 1 - N liczba potomkow , K/M - prośby procesow

pid_t *waitingChild;
int requesters = 0;
int k;
int * signalList;
int *pidList;
int * pidStatus;
int position = 0;


void handlerSIGINT(int y){

    printf("\nI got SIGINT. Closing.\n");
    exit(0);
}

void handlerSIGCHLD(int sig, siginfo_t *info, void *x){

    pidStatus[info->si_pid]=info->si_status;

}

void handlerSIGUSR2(int signal){



    printf("The pid %d got permission. \n ", (int)getpid());

}

void handlerSIGREAL(int sig, siginfo_t *info, void *x){

    sleep(5);
    signalList[position] = sig;
    pidList[position]=(int)info->si_pid;
    position++;


}

void recievedReqest(int signo, siginfo_t *info, void *x){


     printf("The child %d sends reqest.\n", info->si_pid);

    if(requesters < k){
        waitingChild[requesters]=info->si_pid;
        requesters++;

    }
    if(requesters == k ){

        for(int i=0; i<k; i++){

            kill(waitingChild[i], SIGUSR2);
        }
        requesters+=4;
    }
    else if(requesters>k){
       kill(info->si_pid, SIGUSR2);
    }


}

int randomNum(){
    return rand()%30;
}

void createPid(int n, pid_t parent){

    int num;
    struct sigaction sa;
    sa.sa_sigaction = &recievedReqest;
    sigdelset(&sa.sa_mask,SIGUSR1);
    sa.sa_flags = SA_SIGINFO;

    struct sigaction child;
    child.sa_handler = handlerSIGUSR2;
    sigdelset(&child.sa_mask,SIGUSR2);
    child.sa_flags = SA_SIGINFO;

    struct sigaction sigr;
    sigr.sa_sigaction=&handlerSIGREAL;

    struct timespec startRealTime, endRealTime;

    for(int i=SIGRTMIN;i<=SIGRTMAX;i++){
        sigdelset(&sigr.sa_mask,i);
        sigr.sa_flags=SA_SIGINFO;
        sigaction(i,&sigr,NULL);
        }

    if ( sigaction(SIGUSR2, &child, NULL) == -1 ) {
            perror("Couldn't set SIGUSR2 handler");
            exit(1);
        }


    if ( sigaction(SIGUSR1, &sa, NULL) == -1 ) {
                perror("Couldn't set SIGUSR1 handler");
                exit(1);
        }


    for(int i=0; i<n; i++){
        pid_t pid =fork();
        if(pid == 0){
                srand(time(NULL));

            sleep(2);
            clock_gettime(CLOCK_MONOTONIC_RAW, &startRealTime);
            kill(parent, SIGUSR1);

            pause();

            num = rand()%31;
            kill(parent, SIGRTMIN+num);

            clock_gettime(CLOCK_MONOTONIC_RAW, &endRealTime);
            printf("The pid %d is going to finish his life with time %d \n \n",
             (int)getpid(), (int)(endRealTime.tv_sec - startRealTime.tv_sec));

            exit(0); //konczy swoje istnienie i wszystko jest okej

        }
        else if(pid > 0){

            pause();

        }
        else{
        printf("Error during fork().\n");
        exit(1);
        }
    }


}

int main(int argc, char * argv[]){

    if(argc != 3){
        printf("The number of arguments is wrong. \n");
        exit(1);
    }

    signal(2,handlerSIGINT);

    struct sigaction dead;
    dead.sa_sigaction = &handlerSIGCHLD;
    sigdelset(&dead.sa_mask,SIGCHLD);
    dead.sa_flags = SA_SIGINFO;

    sigaction(SIGCHLD, &dead, NULL);


    int numPid = atoi(argv[1]);
    k=atoi(argv[2]);
    pid_t parent = getpid();

    waitingChild = malloc(sizeof(pid_t)*k);
    signalList = malloc(sizeof(int)*numPid);
    pidList = malloc(sizeof(int)*numPid);
    pidStatus = malloc(sizeof(int)*numPid);

    if(numPid <=0 || k <= 0){

        printf("First or second argument is wrong. Enter the number.\n");
        exit(1);
        }


    if(k > numPid){

        printf("Error. The second argument is bigger than first one. \n");
        exit(1);
    }

    createPid(numPid,parent);

    while(pidList[numPid-1] == 0){
        sleep(5);
        if(pidList[numPid-1] != 0) break;
        }
    for(int j=0; j<numPid; j++){

    printf("The PID number %d sent signal number %d. Status is %d. \n",
    pidList[j], signalList[j], pidStatus[j]);

    }


    free(waitingChild);
    free(signalList);
    free(pidList);
    free(pidStatus);


}
