#ifndef HEADERFILE_H
#define HEADERFILE_H

#include <mqueue.h>

#define MAX_LENGTH 100

static const char* SERVER = "/sQueue072";

typedef enum type{
ECHO, VERS, TIME, FINISH, SENDKEY,SENDID, CLOSE
}type;

typedef struct message{
    enum type msgType;
    pid_t pid;
    char messageText[MAX_LENGTH];
    }message;

typedef struct clientID{
	pid_t pid;
	int number;
	mqd_t cQueue;
}clientID;
#endif
