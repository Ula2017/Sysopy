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

pthread_t *threadList;
int recordNum;
char *word;
int threadNum;

int fd;

pthread_mutex_t mutex;
pthread_key_t key;
pthread_mutex_t mutexCancel;



void handlerError(char * message){
    printf(message);
    exit(1);
}

//1-ilosc watkow, 2 - nazwapliku 3 - ilosc rekordow czyanych 4-slowo do wyszukiwania

void* threadStart(void *arg){

    int reader;
    char *findID = malloc(sizeof(char)*10);

    int succ = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if(succ != 0 ) handlerError("Error during setcancelstate.\n");
    succ = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
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
         pthread_testcancel();

         for(int i=0; i < recordNum && i != -2; i++){
            if(strstr(buffer[i], word) != NULL){

                strncpy(findID, buffer[i], 10);
                printf("Search word is in record number %d.\n", atoi(findID));
                printf("TID of thread is %ld.\n", syscall(SYS_gettid));
                fflush(stdout);


                while(pthread_equal(0, threadList[threadNum - 1])!= 0 );
                while(pthread_mutex_trylock(&mutexCancel)!=0) usleep(100);

                for(int j=0; j<threadNum; j++){

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

   pthread_exit(0);

}

int main(int argc, char* argv[]) {

    if(argc !=5){
        handlerError("Wrong number of arguments.\n");
        }

    threadNum = atoi(argv[1]);
    recordNum = atoi(argv[3]);
    char * fileName = argv[2];
    word = argv[4];

    int tmp;

    if(threadNum <= 0 || recordNum <= 0){
        handlerError("Thread number or record number is incorrect.\n");
    }

    fd = open(fileName, O_RDONLY);

    if(fd == -1)
        handlerError("Error during opening file");

    if(pthread_mutex_init(&mutex, NULL) != 0)
        handlerError("Error during init mutex");

     if(pthread_mutex_init(&mutexCancel, NULL) != 0)
        handlerError("Error during init mutexCancel");

    pthread_key_create(&key, NULL);
    threadList =(pthread_t*) malloc(threadNum * sizeof(pthread_t));
    if (threadList == NULL) {
        printf("Error during allocate memory.\n");
        exit(1);
    }

    for(int i=0; i<threadNum; i++){

        tmp = pthread_create(&threadList[i], NULL, threadStart, NULL);

        if(tmp != 0){
            handlerError("Error during creating thread\n");
            }
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
