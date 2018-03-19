#include  <stdlib.h>
#include  <stdio.h>
#include  <sys/msg.h>
#include <sys/ipc.h>
#include "headfile.h"
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>

int clientQueue;

void finish(){
     msgctl(clientQueue, IPC_RMID, 0);
}

int parseLine(char * arg){

    int i;
    if(strcmp("ECHO", arg) ==0) i=0;
    else if(strcmp("VERS", arg) == 0) i=1;
    else if(strcmp("TIME", arg) == 0) i=2;
    else if(strcmp("FINISH", arg) == 0) i=3;
    else if(strcmp("SENDKEY", arg) == 0) i=4;
    else{
    printf("Name of communicate is wrong.\n");
    exit(1);
    }

    return i;
}

int main() {
    atexit(finish);

    key_t cKey = ftok("client", getpid());
    if(cKey == -1){
        printf("Error during clientKey.\n");
        exit(1);
    }

    clientQueue=msgget(cKey, 0666 | IPC_CREAT);
    if(clientQueue == 0)
    {
        printf("Error during create client queue.\n");
        exit(1);
    }

    key_t serverKey = ftok(getenv("HOME"), SERVER);
    int serverQueue;
    message msg;
    if(serverKey == -1){
        printf("Error during serverKey.\n");
        exit(1);
    }

    serverQueue=msgget(serverKey, 0666 | IPC_CREAT);
    if(serverQueue == 0)
    {
        printf("Error during create server queue.\n");
        exit(1);
    }


     char *line = NULL;
    size_t size = 0;
    int result;

    while(1){
        getline(&line, &size, stdin);
        char * typeMS=strtok(line, " \n");
        result = parseLine(typeMS);
        char * arg = strtok(NULL,"\n");

        message m;

        switch(result){
            case 0:

                m.msgType = ECHO;
                strcpy(m.messageText, arg);
                m.pid=getpid();

                if(msgsnd(serverQueue,&m,MAX_LENGTH,0) == -1) {
                    printf("Error during ECHO from %d.\n", getpid());
                    exit(1);

                }
                if(msgrcv(clientQueue,&m,MAX_LENGTH,0,0) == -1){
                    printf("Error during receive message from server.\n");
                    exit(1);
                }
                printf("I reseived from server message ECHO: %s \n", m.messageText);

                break;
            case 1:

                m.msgType = VERS;
                if(strcmp(arg, "") ==0){
                printf("You dont enter message.\n");
                exit(1);
                }
                strcpy(m.messageText, arg);
                m.pid=getpid();

                if(msgsnd(serverQueue,&m,MAX_LENGTH,0) == -1) {
                    printf("Error during VERS from %d.\n", getpid());
                    exit(1);

                }
                if(msgrcv(clientQueue,&m,MAX_LENGTH,0,0) == -1){
                    printf("Error during receive message from server.\n");
                    exit(1);
                }

                printf("I reseived from server upper String: %s \n", m.messageText);
                break;
            case 2:
                m.msgType = TIME;
                m.pid=getpid();

                if(msgsnd(serverQueue,&m,MAX_LENGTH,0) == -1) {
                    printf("Error during TIME from %d.\n", getpid());
                    exit(1);

                }
                if(msgrcv(clientQueue,&m,MAX_LENGTH,0,0) == -1){
                    printf("Error during receive message from server.\n");
                    exit(1);
                }
                printf("Actual time is %s", m.messageText);
                break;
            case 3:
                m.msgType = FINISH;
                m.pid=getpid();

                if(msgsnd(serverQueue,&m,MAX_LENGTH,0) == -1) {
                    printf("Error during FINISH from %d.\n", getpid());
                    exit(1);

                }
                break;
            case 4:
                msg.msgType = SENDKEY;
                msg.pid=getpid();
                sprintf(msg.messageText, "%d\n", (int)cKey);

                if(msgsnd(serverQueue,&msg,MAX_LENGTH,0) == -1) {
                    printf("Error during send request abaut number to server\n");
                    exit(1);

                }

                if(msgrcv(clientQueue,&msg,MAX_LENGTH,0,0) == -1){
                    printf("Error during receive message from server.\n");
                    exit(1);
                }
                printf("I received permission.\n");
                break;
            default:
                printf("Error\n");
                exit(1);


        }
    }


}
