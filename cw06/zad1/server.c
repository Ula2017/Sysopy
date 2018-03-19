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

int serverQueue;
int actualNum=0;
clientID clientList[10];



void finish(){
     msgctl(serverQueue, IPC_RMID, 0);
     for(int i=0; i<actualNum; i++) kill(clientList[i].pid, SIGSTOP);
}

void receiveClientKey(message m){

    int cQueue;
    message response;
    key_t cKey = atoi(m.messageText);

    if((cQueue = msgget(cKey, 0)) == -1){
        printf("Error during create server queue.\n");
        exit(1);
    }

    clientID client;
    if(actualNum>9){
        printf("There is no more place for next clients.\n");
        kill(m.pid, SIGSTOP);
    }
    client.clinetKey =cKey;
    client.number=actualNum;
    client.pid=m.pid;
    client.cQueue=cQueue;
    clientList[actualNum] = client;

    response.pid = getpid();
    response.msgType = SENDID;
    sprintf(response.messageText, "%d", actualNum);

    if(msgsnd(cQueue,&response,MAX_LENGTH,0) == -1) {
        printf("Error during send response to %d\n", (int)client.pid);
        exit(1);

    }

    actualNum++;


}

int isClientConnect(pid_t pid){

    for(int i=0; i<actualNum; i++){
        if(clientList[i].pid == pid) return clientList[i].cQueue;
    }

    return -1;

}

void receiveString(message m){

    message response;
    int que=isClientConnect(m.pid);
    if(que == -1) {
        printf("Client %d isnt connect.", (int)m.pid);
        kill(m.pid, SIGSTOP);
        return;
    }

    response.pid=getpid();
    response.msgType=SENDID;
    strcpy(response.messageText, m.messageText);

    if(msgsnd(que,&response,MAX_LENGTH,0) == -1) {
        printf("Error during resend ECHO.\n");
        exit(1);

    }


}

void changeToUper(message m){

    message response;
    int que=isClientConnect(m.pid);
    if(que == -1) {
        printf("Client %d isnt connect.", (int)m.pid);
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


    if(msgsnd(que,&response,MAX_LENGTH,0) == -1) {
        printf("Error during resend ECHO.\n");
        exit(1);

    }

}

void sendTime(message m){

    message response;
    int que=isClientConnect(m.pid);
    if(que == -1) {
        printf("Client %d isnt connect.", (int)m.pid);
        kill(m.pid, SIGSTOP);
        return;
        }
    printf("client connected");
    time_t t;
    time(&t);

    response.pid=getpid();
    response.msgType =SENDID;
    strcpy(response.messageText, asctime( localtime( &t) ));

    if(msgsnd(que,&response,MAX_LENGTH,0) == -1) {
        printf("Error during send TIME.\n");
        exit(1);

    }


}

void terminateServer (message m){

    int k=1;
    while(k==1){
            struct msqid_ds ds;
            msgctl(serverQueue, IPC_STAT, &ds);

            if((int)ds.msg_qnum == 0) k=0;

            else{
                struct message msg;
                msgrcv(serverQueue, &msg, MAX_LENGTH, 0, IPC_NOWAIT);
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
    exit(0);

}

int main(){

    atexit(finish);

    key_t serverKey = ftok(getenv("HOME"), SERVER);
    if(serverKey == -1){
        printf("Error during serverKey.\n");
        exit(1);
    }
    struct message msg;


    serverQueue=msgget(serverKey, 0666 | IPC_CREAT);

    if(serverQueue == 0)
    {
        printf("Error during create server queue.\n");
        exit(1);
    }

    printf("Server connected successfully.\n");

    while(1){

        if(msgrcv(serverQueue,&msg,MAX_LENGTH,0,0) == -1){
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
            default:
                printf("Message is ignored because of wrong type.\n");
                break;


        }


      }



      return 0;
}
