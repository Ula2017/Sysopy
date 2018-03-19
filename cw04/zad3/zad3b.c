#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<signal.h>
#include<string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

pid_t *pid;
int pidNum;
int counter=0;

void handlerSIGINT(int signal){

    puts("\nI got SIGINT. Closing.\n");
    for(int i=0; i<pidNum; i++) kill(pid[i], SIGINT);
    exit(1);
}

void handlerSIGUSR1(int signal){

    counter++;
}

void handlerSIGREAL1(int signal){

    counter++;
}


int main(int argc, char* argv[]){
    srand(time(NULL));
    signal(2,handlerSIGINT);

    if(argc != 3 && argc !=4){

        printf("The number of arguments is wrong. \n");
        exit(1);
    }
    int t;

    if(argc == 3 ) t=0;
    else t = atoi(argv[3]);

    pidNum = atoi(argv[1]);
    int signalNum = atoi(argv[2]);
    pid = malloc(sizeof(pid_t)*pidNum);
    int type;
    int user1counter;
    char *arguments[2];
    arguments[0] ="./pid";
    arguments[1] = NULL;
    int tmp;
    pid_t child;

    if(pidNum <= 0 && signalNum <= 0){

        printf("First or second argument is wrong. \n");
        exit(1);
    }

    signal(SIGUSR1, &handlerSIGUSR1);

    for(int i=0; i<pidNum; i++){
        pid[i]=fork();
        if(pid[i] == 0){
            execvp(arguments[0], arguments);
        }
    }

    for(int j=0; j<signalNum/2; j++){

        user1counter=0;
        type=(rand()%3)+1;
        tmp = rand()%pidNum;
        sleep(t);
        child=pid[tmp];




    switch(type){
        case 1: // funkcja kill
            sleep(1);
            for(int i=0; i<signalNum; i++){

                    kill(child, SIGUSR1 );
                    sleep(1);
                    user1counter++;

            }

                kill(child, SIGUSR2);
            break;

        case 2:
             sleep(1);

            union sigval sigvalue;
            sigvalue.sival_int=0;

            for(int i=0; i<signalNum; i++){


                if(sigqueue(child, SIGUSR1, sigvalue) == 1){
                    printf("Error during sigqueue. \n");
                    exit(1);
                }
                sleep(1);
                user1counter++;

            }

                if(sigqueue(child, SIGUSR2, sigvalue) == 1){
                    printf("Error during sigqueue. \n");
                    exit(1);
                }
            break;

        case 3:
                signal(SIGRTMIN+3, &handlerSIGREAL1);
                sleep(1);
                for(int i=0; i<signalNum; i++){

                    kill(child, SIGRTMIN+3 );
                    sleep(1);
                    user1counter++;

                }

                 kill(child, SIGRTMIN+10);
            break;

        default:
            printf("Uncorrect second argument. Usage: 1,2,3. \n");
            exit(1);

    }
    printf("The process %d sent %d SIGUSR1  to %d.\n", (int)getpid(), user1counter, (int)child );
    printf("The parent received %d SIGUSR1 from %d. \n", counter, child);
    counter = 0;

    }


    return 0;
}
