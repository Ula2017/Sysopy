#ifndef HEADERFILE_H
#define HEADERFILE_H

#define MAX_LENGTH 100

static const int SERVER = 50;

typedef enum type{
ECHO, VERS, TIME, FINISH, SENDKEY,SENDID
}type;

typedef struct message{
    enum type msgType;
    pid_t pid;
    char messageText[MAX_LENGTH];
    }message;

typedef struct clientID{
	pid_t pid;
	key_t clinetKey;
	int number;
	int cQueue;
}clientID;
#endif
