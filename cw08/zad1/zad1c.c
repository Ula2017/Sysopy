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



void handlerError(char * message){
    printf(message);
    exit(1);
}

//1-ilosc watkow, 2 - nazwapliku 3 - ilosc rekordow czyanych 4-slowo do wyszukiwania

void* threadStart(void *arg){

    int reader;
    char *findID = malloc(sizeof(char)*10);

    int succ = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    if(succ != 0 ) handlerError("Error during setcancelstate.\n");

    char *buffer[recordNum];
    for(int i=0; i<recordNum; i++) buffer[i] = malloc(sizeof(char)*1024);

    if(pthread_setspecific(key, buffer) !=0) handlerError("Error during set thread local.\n");

    int continueProg =1;
    while(continueProg){

         pthread_mutex_lock(&mutex);
         for(int i=0; i < recordNum; i++){
            reader = read(fd, buffer[i], 1024);
            if(reader == -1 ) handlerError("Error during read file.\n");
            if(reader == 0) continueProg = 0;
         }

         pthread_mutex_unlock(&mutex);

         for(int i=0; i < recordNum && i != -1; i++){
            if(strstr(buffer[i], word) != NULL){

                strncpy(findID, buffer[i], 10);
                printf("Search word is in record number %s.\n", findID);
                printf("TID of thread is %ld.\n", syscall(SYS_gettid));
                fflush(stdout);

                i = -3;

            }

        }
        }

    for(int i=0; i<recordNum; i++) free(buffer[i]);
    free(findID);
    pthread_exit(NULL);

}

int main(int argc, char* argv[]) {

    if(argc !=5)
        handlerError("Wrong number of arguments.\n");


    threadNum = atoi(argv[1]);
    recordNum = atoi(argv[3]);
    char * fileName = argv[2];
    word = argv[4];

    int tmp;

    if(threadNum <= 0 || recordNum <= 0)
        handlerError("Thread number or record number is incorrect.\n");


    fd = open(fileName, O_RDONLY);

    if(fd == -1)
        handlerError("Error during opening file");

    if(pthread_mutex_init(&mutex, NULL) != 0)
        handlerError("Error during init mutex");

    pthread_key_create(&key, NULL);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    threadList =(pthread_t*) malloc(threadNum * sizeof(pthread_t));
    if (threadList == NULL)
        handlerError("Error during allocate memory.\n");

    for(int i=0; i<threadNum; i++){

        tmp = pthread_create(&threadList[i], &attr, threadStart, NULL);

        if(tmp != 0)
            handlerError("Error during creating thread\n");


    }


    free(threadList);
    pthread_exit(0); //czeamy na wszystkie watki
    close(fd);
    pthread_attr_destroy(&attr);
    pthread_key_delete(key);
    pthread_mutex_destroy(&mutex);
}
