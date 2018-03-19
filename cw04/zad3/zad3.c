#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<signal.h>
#include<string.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t pid;
int counter=0;

void handlerSIGINT(int signal){

    puts("\nI got SIGINT. Closing.\n");
    kill(pid, SIGINT);
    exit(1);
}

void handlerSIGUSR1(int signal){

    counter++;
}

void handlerSIGREAL1(int signal){

    counter++;
}


int main(int argc, char* argv[]){

    signal(2,handlerSIGINT);

    if(argc != 3){

        printf("The number of arguments is wrong. \n");
        exit(1);
    }

    int signalNum = atoi(argv[1]);
    int type = atoi(argv[2]);
    int user1counter=0;
    char *arguments[2];
    arguments[0] ="./pid";
    arguments[1] = NULL;

    if(signalNum <= 0){

        printf("First argument is wrong. \n");
        exit(1);
    }

    switch(type){
        case 1: // funkcja kill

            pid = fork();
            if(pid == 0){
                execvp(arguments[0], arguments);

            }
            else if(pid > 0){

                signal(SIGUSR1, &handlerSIGUSR1);
                sleep(1);

                for(int i=0; i<signalNum; i++){

                    kill(pid, SIGUSR1 );
                    sleep(1);
                    user1counter++;

                }

                kill(pid, SIGUSR2);

            }
            else{
                printf("Error during fork().\n");
                exit(1);
            }
            break;
        case 2:
            pid = fork();
            if(pid == 0){

                execvp(arguments[0], arguments);

            }
            else if(pid > 0){

                signal(SIGUSR1, &handlerSIGUSR1);
                sleep(1);
                union sigval sigvalue;
                sigvalue.sival_int=0;

                for(int i=0; i<signalNum; i++){


                    if(sigqueue(pid, SIGUSR1, sigvalue) == 1){
                        printf("Error during sigqueue. \n");
                        exit(1);
                    }
                    sleep(1);
                    user1counter++;

                }

                if(sigqueue(pid, SIGUSR2, sigvalue) == 1){
                    printf("Error during sigqueue. \n");
                    exit(1);
                }

            }
            else{
                printf("Error during fork().\n");
                exit(1);
            }
            break;
        case 3:
            pid = fork();
            if(pid == 0){
                execvp(arguments[0], arguments);

            }
            else if(pid > 0){

                signal(SIGRTMIN+3, &handlerSIGREAL1);
                sleep(1);
                for(int i=0; i<signalNum; i++){

                    kill(pid, SIGRTMIN+3 );
                    sleep(1);
                    user1counter++;

                }

                 kill(pid, SIGRTMIN+10);
                }
            else{
                printf("Error during fork().\n");
                exit(1);
            }
            break;
        default:
            printf("Uncorrect second argument. Usage: 1,2,3. \n");
            exit(1);

    }
    printf("The process %d sent %d SIGUSR1 \n", (int)getpid(), user1counter);
    printf("The parent received %d SIGUSR1 from child. \n", counter);

    return 0;
}
