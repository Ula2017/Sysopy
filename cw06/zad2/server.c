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
int actualNum=0;
clientID clientList[10];



void finish(){
    for(int i=0; i < actualNum; i++) mq_close(clientList[i].cQueue);

}

void closeServer(){
    if(mq_close(serverQueue) == -1){
        printf("Error during close server queue.\n");
        exit(1);
    }
    if(mq_unlink(SERVER) == -1){
        printf("Error during delete server queue.\n");
        exit(1);
    }
    exit(0);
}

void receiveClientKey(message m){

    mqd_t cQueue;
    message response;
    char* cKey = m.messageText;

    if(actualNum>9){
        printf("There is no more place for next clients.\n");
        kill(m.pid, SIGSTOP);
        return;
    }

    if((cQueue = mq_open(cKey, O_WRONLY)) == -1){
        printf("Error during open client queue.\n");
        exit(1);
    }

    clientID client;
    client.number=actualNum;
    client.pid=m.pid;
    client.cQueue=cQueue;
    clientList[actualNum] = client;

    response.pid = getpid();
    response.msgType = SENDID;
    sprintf(response.messageText, "%d", actualNum);

    if(mq_send(cQueue, (char*)&response, MAX_LENGTH, 1 ) == -1) {
        printf("Error during send response to %d\n", (int)client.pid);
        exit(1);

    }

    actualNum++;


}
mqd_t isClientConnect(pid_t pid){

    for(int i=0; i<actualNum; i++){
        if(clientList[i].pid == pid) return clientList[i].cQueue;
    }

    return -1;

}

void receiveString(message m){

    message response;
    mqd_t que=isClientConnect(m.pid);
    if(que == -1) {
    kill(m.pid, SIGSTOP);
    return;
    }


    response.pid=getpid();
    response.msgType=SENDID;
    strcpy(response.messageText, m.messageText);

    if(mq_send(que, (char*)&response, MAX_LENGTH, 1 ) == -1) {
        printf("Error during send response to %d\n", (int)m.pid);
        exit(1);

    }


}

void changeToUper(message m){

    message response;
    mqd_t que=isClientConnect(m.pid);
    if(que == -1) {
    kill(m.pid, SIGSTOP);
    return;
    }

    response.pid=getpid();
    response.msgType=SENDID;
    int i=0;
    while(m.messageText[i] != '\0'){
        response.messageText[i] = toupper(m.messageText[i]);
        i++;
    }
    response.messageText[i]= '\0';


    if(mq_send(que, (char*)&response, MAX_LENGTH, 1 ) == -1) {
        printf("Error during send response to %d\n", (int)m.pid);
        exit(1);

    }

}

void sendTime(message m){

    message response;
    mqd_t que=isClientConnect(m.pid);
    if(que == -1) {
    kill(m.pid, SIGSTOP);
    return;
    }

    time_t t;
    time(&t);

    response.pid=getpid();
    response.msgType =SENDID;
    strcpy(response.messageText, asctime( localtime( &t) ));

    if(mq_send(que, (char*)&response, MAX_LENGTH, 1 ) == -1) {
        printf("Error during send response to %d\n", (int)m.pid);
        exit(1);

    }


}

void terminateServer (message m){

    mqd_t que=isClientConnect(m.pid);
    if(que == -1) {
        kill(m.pid, SIGSTOP);
        return;
    }

    int k=1;
    while(k==1){
            struct mq_attr mqAttr;
            if(mq_getattr(serverQueue, &mqAttr) ==-1){
            printf("Error during read mq_attr.\n");
            exit(1);
            }

            if(mqAttr.mq_curmsgs == 0) k=0;

            else{
                struct message msg;
                unsigned int priorityQueue =1;

                if(mq_receive(serverQueue, (char*)&msg, MAX_LENGTH, &priorityQueue ) == -1){
                    printf("Error during receive message from client.\n");
                    exit(1);
                }

                enum type messageType = msg.msgType;
                switch(messageType){

                    case ECHO:
                        receiveString(msg);
                        break;
                    case VERS:
                        changeToUper(msg);
                        break;
                    case TIME:
                        sendTime(msg);
                        break;
                    case FINISH:
                        break;
                    case SENDKEY:
                        receiveClientKey(msg);
                        break;
                    default:
                        printf("Message is ignored because of wrong type.\n");
                        break;

                }
            }
    }


    printf("Terminate Server.\n");
    finish();
    exit(0);

}
void closeClientQueue(pid_t pid){
    int tmp;
    mqd_t que=isClientConnect(pid);
    if(que == -1) {
    kill(pid, SIGSTOP);
    return;
    }
    for(int i=0; i<actualNum; i++){
        if(clientList[i].pid == pid) {
            mq_close(clientList[i].cQueue);
            tmp=i;
            break;
            }
    }

    while(tmp!=actualNum){
    clientList[tmp]=clientList[tmp+1];
    tmp++;
    }
    actualNum--;



}

int main(){

    atexit(closeServer);
    struct mq_attr mqAttr;
    mqAttr.mq_msgsize=MAX_LENGTH;
    mqAttr.mq_maxmsg =10;

    serverQueue = mq_open(SERVER,  O_CREAT | O_RDONLY, 0666, &mqAttr);
    if(serverQueue == -1)
    {
        printf("Error during create server queue.\n");
        exit(1);
    }

    printf("Server connected successfully.\n");

    while(1){

        message msg;

        unsigned int priorityQueue =1;
        if(mq_receive(serverQueue, (char*)&msg, MAX_LENGTH, &priorityQueue ) == -1){
        printf("Error during receive message from client.\n");
        exit(1);
        }

        enum type messageType = msg.msgType;
        switch(messageType){

            case ECHO:
                receiveString(msg);
                break;
            case VERS:
                changeToUper(msg);
                break;
            case TIME:
                sendTime(msg);
                break;
            case FINISH:
                terminateServer(msg);
                break;
            case SENDKEY:
                receiveClientKey(msg);
                break;
            case CLOSE:
                closeClientQueue(msg.pid);
                break;
            default:
                printf("Message is ignored because of wrong type.\n");
                break;


        }

      }



      return 0;
}
