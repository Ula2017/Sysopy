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
#include <signal.h>
#include <mqueue.h>
#include <sys/stat.h>

mqd_t serverQueue;
mqd_t clientQueue;

void finish(){
    if(mq_close(clientQueue) == -1){
        printf("Error during close client queue.\n");
        exit(1);
    }
    if(mq_unlink("/client") == -1){
        printf("Error during delete client queue.\n");
        exit(1);
    }

}

int parseLine(char * arg){

    int i;
    if(strcmp("ECHO", arg) ==0) i=0;
    else if(strcmp("VERS", arg) == 0) i=1;
    else if(strcmp("TIME", arg) == 0) i=2;
    else if(strcmp("FINISH", arg) == 0) i=3;
    else if(strcmp("SENDKEY", arg) == 0) i=4;
    else if(strcmp("CLOSE", arg) == 0) i=5;
    else{
    printf("Name of communicate is wrong.\n");
    exit(1);
    }

    return i;
}

int main() {
    atexit(finish);

    struct mq_attr mqAttr;
    mqAttr.mq_msgsize=MAX_LENGTH;
    mqAttr.mq_maxmsg =10;
    unsigned int priorityQueue;

    clientQueue = mq_open("/client", O_RDONLY | O_CREAT, 0666, &mqAttr);
    if(clientQueue == -1)
    {
        printf("Error during create client queue.\n");
        exit(1);
    }

    if((serverQueue = mq_open(SERVER, O_WRONLY)) == -1){
        printf("Error during open server queue.\n");
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

                if(mq_send(serverQueue, (char*)&m, MAX_LENGTH, 1 ) == -1) {
                    printf("Error during send ECHO message to server from %d\n", (int)getpid());
                    exit(1);
                }

                if(mq_receive(clientQueue, (char*)&m, MAX_LENGTH, &priorityQueue ) == -1){
                    printf("Error during receive ECHO message from server.\n");
                    exit(1);
                }

                printf("I reseived from server message ECHO: %s\n", m.messageText);

                break;
            case 1:

                m.msgType = VERS;
                strcpy(m.messageText, arg);
                m.pid=getpid();

                if(mq_send(serverQueue, (char*)&m, MAX_LENGTH, 1 ) == -1) {
                    printf("Error during send VERS message to server from %d.\n", getpid());
                    exit(1);
                }

                if(mq_receive(clientQueue, (char*)&m, MAX_LENGTH, &priorityQueue ) == -1){
                    printf("Error during receive VERS message from server.\n");
                    exit(1);
                }

                printf("I reseived from server upper String: %s\n", m.messageText);
                break;
            case 2:
                m.msgType = TIME;
                m.pid=getpid();

                if(mq_send(serverQueue, (char*)&m, MAX_LENGTH, 1 ) == -1) {
                    printf("Error during send TIME message to server from %d.\n", getpid());
                    exit(1);

                }
                if(mq_receive(clientQueue, (char*)&m, MAX_LENGTH, &priorityQueue ) == -1){
                    printf("Error during receive TIME message from server.\n");
                    exit(1);
                }
                printf("Actual time is %s", m.messageText);
                break;
            case 3:
                m.msgType = FINISH;
                m.pid=getpid();

                if(mq_send(serverQueue, (char*)&m, MAX_LENGTH, 1 ) == -1) {
                    printf("Error during send FINISH message to server from %d.\n", getpid());
                    exit(1);

                }
                break;
            case 4:
                m.msgType = SENDKEY;
                m.pid=getpid();
                strcpy(m.messageText, "/client");

                if(mq_send(serverQueue, (char*)&m, MAX_LENGTH, 1 )== -1) {
                    printf("Error during send SENDKEY message to server\n");
                    exit(1);

                }

                if(mq_receive(clientQueue, (char*)&m, MAX_LENGTH, &priorityQueue ) == -1){
                    printf("Error during receive SENDIN message from server.\n");
                    exit(1);
                }
                printf("Client %d connected successfully.\n", getpid() );
                break;
            case 5:
                m.msgType = CLOSE;
                m.pid=getpid();

                if(mq_send(serverQueue, (char*)&m, MAX_LENGTH, 1 )== -1) {
                    printf("Error during send CLOSE message to server\n");
                    exit(1);

                }
                exit(0);
                break;
            default:
                printf("Error\n");
                exit(1);


        }
    }
    return 0;


}
