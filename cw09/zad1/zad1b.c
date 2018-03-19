#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/syscall.h>

int * tab;
int size;
pthread_t *threadList;

pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex;
//pthread_mutex_t readerMutex ;

//zad1b: ../nptl/pthread_mutex_lock.c:81: __pthread_mutex_lock: Assertion `mutex->__data.__owner == 0' failed.



int isWriter =0;
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

        isWriter++;
        if(pthread_mutex_lock(&mutex) != 0 )
            handlerError("Error: locking mutex failed.\n");

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
        isWriter--;
        pthread_cond_broadcast(&cond);
       if(pthread_mutex_unlock(&mutex) != 0 )
            handlerError("Error: unlocking mutex failed.\n");

        goToSleepForWhile();
        j = 0;

   }

    return 0;
}

// Reader thread function
int readerFun(void* data) {

    int divider = *(int*) data;
    int resultTab[600][2];
    int found =0;

    while(1){
       while(isWriter > 0){
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_trylock(&mutex); //po prostu sprobuj - jesli sie nie uda to nic

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

        if(pthread_mutex_unlock(&mutex) != 0 )
            handlerError("Error: unlocking mutex failed.\n");

        goToSleepForWhile();
        found = 0;

    }

    return 0;
}

//1 - liczba pisarzy, 2 liczba czytelnikow ,3 - dzielnik 4 - 1 opcja -i , 0 - bez opcji -i
//funkcja faworyzujaca pisarzy
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

    createTab();

    if(pthread_mutex_init(&mutex, NULL) != 0)
        handlerError("Error during init mutex");

/*    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);

    if(pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE ) != 0)
        handlerError("Error: Setting type of mutex failed.\n");

    if(pthread_mutex_init(&readerMutex, &attr) != 0)
        handlerError("Error during init mutex"); */



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

    if(pthread_mutex_destroy(&mutex) != 0)
        handlerError("Error: destruction of mutex failed.\n");
//    if(pthread_mutex_destroy(&readerMutex) != 0)
//        handlerError("Error: destruction of mutex failed.\n");

//    if(pthread_mutexattr_destroy(&attr) != 0 )
//        handlerError("Error: destruction of mutex attr failed.\n");
     pthread_cond_destroy(&cond);

    return (0);


}
