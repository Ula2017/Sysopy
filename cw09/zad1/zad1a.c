#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include<semaphore.h>
#include <time.h>
#include <sys/types.h>
#include <sys/syscall.h>

int * tab;
int size;
pthread_t *threadList;

sem_t readerSem;
sem_t writerSem;

int version = 0;
int readers;

void handlerError(char * message){
    printf(message);
    exit(1);
}

void createTab(){
    size = rand() % 301;
    tab = malloc(sizeof(int)*size);

    for(int i=0; i<size; i++) tab[i] = (int)(-300 + rand() / (RAND_MAX + 1.0) * 600);

}
void goToSleepForWhile(){
    int time = rand() % 500;
    usleep(time);

}

// Writer thread function
int writerFun(void* data) {

    int j=0;
    int numToChange;
    int num;
    int pos;

   while(1){

        if( sem_wait(&writerSem) != 0 )
            handlerError("(W) Error: waiting for writing semaphore failed.\n");
        printf("(W) Writer started writing...\n");

        numToChange = (int)(rand() / (RAND_MAX + 1.0) *20);
        int resultTab[numToChange][2];

        for(int i =0; i< numToChange; i++){

            pos = (int)(rand() / (RAND_MAX + 1.0) *size); //losujemy pozycje do zmiany
            num =(int)(-300 + rand() / (RAND_MAX + 1.0) * 600);
            tab[pos] = num;
            if(version == 1){
                resultTab[j][0] = pos;
                resultTab[j][1] = num;
                j++;
                }
        }
        printf("(W) Just finish.\n");
        if(version == 1){
            printf("(W) Extension version.\n");
            for(int i =0; i<j; i++)
                printf("index: %d, value: %d \n", resultTab[i][0], resultTab[i][1]);

                }
        if(sem_post(&writerSem) != 0)
            handlerError("Error:  incrementation of writing semaphore failed.\n");
        goToSleepForWhile();
        j = 0;

   }

    return 0;
}

// Reader thread function
int readerFun(void* data) {

    int divider = *(int*) data;
    int tmp;
    int resultTab[600][2];
    int found =0;

    while(1){
        if( sem_trywait(&writerSem) == -1 ){
            sem_getvalue(&readerSem, &tmp);
            if(tmp == readers) // znaczy ze pisarz zajal
                sem_wait(&writerSem);
            }

        if( sem_wait(&readerSem) != 0 )
            handlerError("(R) Error: waiting for reading semaphore failed.\n");
        printf("(R) Reader started reading...\n");

        for(int i =0; i< size; i++){

            if(tab[i] % divider == 0 ){
                found++;
                if(version == 1){
                    resultTab[found][0] = i;
                    resultTab[found][1] = tab[i];

                }
            }
        }
        printf("(R) Reader %ld reports: \n -found %d dividers.\n",syscall(SYS_gettid), found);
        if(version == 1){
            printf("(R) Extension version.\n");
            for(int i =0; i<found; i++)
                printf("index: %d, value: %d \n", resultTab[i][0], resultTab[i][1]);
                }
        if(sem_post(&readerSem) != 0)
            handlerError("Error:  incrementation of reading semaphore failed.\n");
        sem_getvalue(&readerSem, &tmp);

        if(tmp == readers){
            if(sem_post(&writerSem) != 0)
            handlerError("Error:  incrementation of writing semaphore failed.\n");
            }

        goToSleepForWhile();
        found = 0;

    }

    return 0;
}

//1 - liczba pisarzy, 2 liczba czytelnikow ,3 - dzielnik 4 - opcja -i , albo bez  4 parametru
int main(int argc, char* argv[]) {

    srand(time(0));

    if(argc != 4 && argc != 5)
        handlerError("Wrong number of argument.\n");


    int writers = atoi(argv[1]);
    readers = atoi(argv[2]);
    int * divider = malloc(sizeof(int));
    *divider = atoi(argv[3]);

    if(argc == 5){
        char * option =argv[4];

        if(strcmp(option, "-i" ) != 0)
            handlerError("Error. Option can be only -i.\n");
        else version = 1;
        }

    int tmp;
    threadList =(pthread_t*) malloc((writers + readers) * sizeof(pthread_t));



    if(writers <= 0 || readers <= 0 || &divider <= 0)
        handlerError("Argument should be number.\n");

    if(sem_init(&readerSem, 0, readers) == -1)
        handlerError("Error: Initialization of the reading semaphore failed.n");

    if(sem_init(&writerSem, 0, 1) == -1)
        handlerError("Error: Initialization of the writing semaphore failed.n");

    createTab();


    for ( int i=0; i< readers; i++){
        tmp = pthread_create(&threadList[i], NULL, (void*)readerFun, (void*)divider);
        if(tmp != 0)
            handlerError("Error during creating  reader thread\n");
            }

    for ( int i=0; i< writers; i++){
        tmp = pthread_create(&threadList[i+readers], NULL, (void*)writerFun, NULL);
        if(tmp != 0)
            handlerError("Error during creating  reader thread\n");
            }

    // Wait for the Readers and writers
    for (int i = 0; i < readers+writers; i++)
        pthread_join(threadList[i],NULL);



    free(divider);
    free(threadList);
    free(tab);
    if(sem_destroy(&readerSem) == -1)
        handlerError("Error: removing reading semaphore failed.\n");
    if(sem_destroy(&writerSem) == -1)
        handlerError("Error: removing writing semaphore failed.\n");

    return (0);


}
