#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <signal.h>

pthread_t *threadList;
int recordNum;
char *word;
int threadNum;

int fd;

int sig;
int option;

pthread_mutex_t mutex;
pthread_mutex_t mutexCancel;
pthread_key_t key;

void handlerError(char * message){
    printf(message);
    exit(1);
}

void signalHandler(int c){
    printf("The signal was caught. PID: %d, TID: %ld \n ", getpid(),syscall(SYS_gettid) );
}


//1-ilosc watkow, 2 - nazwapliku 3 - ilosc rekordow czyanych 4-slowo do wyszukiwania
//5 - opcja 1-5 odnosnie sygnałów, 6 - dzielnie przez zero
// 6 - opcja odnosnie ktorego sygnalu SIGUSR1, SIGTERM, SIGKILL, SIGSTOP.

void* threadStart(void *arg){

    sleep(1);
    if(option == 4){

        sigset_t set;
		sigemptyset(&set);
		sigaddset(&set, sig);
		int tmp = pthread_sigmask(SIG_SETMASK, &set, NULL);
		if(tmp  < 0 )
            handlerError("Error during set mask for .\n");

        sleep(1);
	}

    if(option == 5){

        if(sig != SIGKILL && sig != SIGSTOP ){
                if(signal(sig, signalHandler) == SIG_ERR )
                    handlerError("Error during signal in thread.\n");
                }
	}

	if(option == 6 && pthread_equal(pthread_self(), threadList[1])!=0){
        int a = 2/(option-6);
        printf("The test with devide by 0: %d", a);
	}



    int reader;
    char *findID = malloc(sizeof(char)*10);

    int succ = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if(succ != 0 ) handlerError("Error during setcancelstate.\n");
    succ = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    if(succ != 0 ) handlerError("Error during cancelasynchronous.\n");

    char *buffer[recordNum];
    for(int i=0; i<recordNum; i++) buffer[i] = malloc(sizeof(char)*1024);

    if(pthread_setspecific(key, buffer) !=0) handlerError("Error during set thread local.\n");

    int continueProg =1;
    while(continueProg){

         while(pthread_mutex_trylock(&mutex)!=0) usleep(100);
         for(int i=0; i < recordNum; i++){
            reader = read(fd, buffer[i], 1024);
            if(reader == -1 ) handlerError("Error during read file.\n");
            if(reader == 0) continueProg = 0;
         }

         pthread_mutex_unlock(&mutex);

         for(int i=0; i < recordNum && i != -1; i++){
            if(strstr(buffer[i], word) != NULL){

                strncpy(findID, buffer[i], 10);
                printf("Search word is in record number %d.\n", atoi(findID));
                printf("TID of thread is %ld.\n", syscall(SYS_gettid));
                fflush(stdout);


                while(pthread_equal(0, threadList[threadNum - 1])!= 0 );
                while(pthread_mutex_trylock(&mutexCancel)!=0) usleep(100);
                for(int j=0; j<threadNum && threadList[j] != 0 ; j++){

                  if(pthread_equal(pthread_self(), threadList[j])==0)
                        pthread_cancel(threadList[j]);

                }


                i = -3;
               continueProg = 0;

            }

        }
        }

   for(int i=0; i<recordNum; i++) free(buffer[i]);

   free(findID);
    return NULL;


}

int main(int argc, char* argv[]) {

    if(argc < 6 || argc > 7){
        handlerError("Wrong number of arguments.\n");
        }

    threadNum = atoi(argv[1]);
    recordNum = atoi(argv[3]);
    char * fileName = argv[2];
    word = argv[4];
    option = atoi(argv[5]);
    sigset_t signalMask;

    if(option != 6 && argc == 7){
        if (strcmp(argv[6], "sigusr1") == 0) sig= SIGUSR1;
        else if (strcmp(argv[6], "sigterm") == 0) sig = SIGTERM;
        else if (strcmp(argv[6], "sigkill") == 0) sig = SIGKILL;
        else if (strcmp(argv[6], "sigstop") == 0)sig = SIGSTOP;
        else handlerError("Singal name is wrong.\n");
        }
    else if(option != 6 && argc != 7)
        handlerError("For option 1 to 5 you need to enter 7th arguments.\n");


    if(threadNum <= 0 || recordNum <= 0)
        handlerError("Thread number or record number is incorrect.\n");



    fd = open(fileName, O_RDONLY);

    if(fd == -1){
        handlerError("Error during opening file");
        }

    if(pthread_mutex_init(&mutex, NULL) != 0)
        handlerError("Error during init mutex");

     if(pthread_mutex_init(&mutexCancel, NULL) != 0)
        handlerError("Error during init mutexCancel");

    pthread_key_create(&key, NULL);
    threadList =(pthread_t*) malloc(threadNum * sizeof(pthread_t));
    int tmp;

    if (threadList == NULL)
        handlerError("Error during allocate memory.\n");



    for(int i=0; i<threadNum; i++){

        tmp = pthread_create(&threadList[i], NULL, threadStart, NULL);
        if(tmp != 0)
            handlerError("Error during creating thread\n");

    }

    switch(option){
        case 1: //zaden watek nie ma zamaskowanego sygnalu
            kill(getpid(), sig);
            break;
        case 2: //glowny watek ma zamaskowany sygnal
            sigemptyset(&signalMask);
            sigaddset(&signalMask, sig);
            tmp = pthread_sigmask(SIG_SETMASK, &signalMask, NULL);
            if(tmp < 0)
                handlerError("Error during setmask in case 2 .\n");
            kill(getpid(), sig);
            break;
        case 3:
            if(sig != SIGKILL && sig != SIGSTOP ){
                if(signal(sig, signalHandler) == SIG_ERR )
                    handlerError("Error during signal.\n");
                }

            kill(getpid(), sig);
            break;
        case 4:
        sleep(2);
            tmp = pthread_kill(threadList[0], sig);
            if(tmp != 0 )
                handlerError("Error during kill thread in option 4 .\n");
            break;
        case 5:
            sleep(1);
            tmp = pthread_kill(threadList[0], sig);
            if(tmp != 0 )
                handlerError("Error during kill thread in option 5.\n");
            break;
        case 6:
            break;
        default:
            handlerError("Signal number is from 1 to 4.\n");
            }

    for (int i = 0; i < threadNum; i++) {

        tmp = pthread_join(threadList[i], NULL);
        if (tmp != 0)
                handlerError("Error during joining thread\n");

           }

    close(fd);
    free(threadList);
    pthread_key_delete(key);
    pthread_mutex_unlock(&mutexCancel);
    pthread_mutex_destroy(&mutexCancel);
    pthread_mutex_destroy(&mutex);
    printf("Main program is finishing his life right now.\n");
    fflush(stdout);

    return 0;
}
