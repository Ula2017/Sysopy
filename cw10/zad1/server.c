#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include<sys/epoll.h>

int maxClient = 15;
int th = 0;
int sockUnix;
int sockfd;
pthread_t thread;
in_port_t port;
char * path;
int counter=0;
int clientNum =0;

struct message{
	int type;
	int arg[2];
	char name[100];
};

struct clientInfo{
	char * name;
	int fd;
};

struct clientInfo users[15];
fd_set readfds;


int efd = -1;

void exiting(){

	if (shutdown(sockUnix, SHUT_RDWR) == -1)
			puts("Error during shutting down socket 2.");

	if (close(sockUnix) == -1)
			puts("Error during closing socket 2.");

	if (shutdown(sockfd, SHUT_RDWR) == -1)
			puts("Error during shutting down socket 1.");

	if (close(sockfd) == -1)
			puts("Error during closing socket 1.");

//kill(getpid(), SIGKILL);

if(th == 1){
if (pthread_cancel(thread) == -1)
		puts("Error during canceling thread.");

if (pthread_join(thread, NULL)== -1)
		puts("Error during joining thread.");

}



}
void handlerSIGINT(int x){
				exit(0);

}

void handlerError(char * message){
    printf(message);
		exit(1);
}

int chooseClient(){
	if(clientNum == 0){
		 return -1;
	 }

	int num= rand()%clientNum;
	int filedes = users[num].fd;
 	return filedes;

}
void sendingMsg(struct message msg){

	int x = chooseClient();
	if(x == -1)
		handlerError("Error during searching client fd.\n");

	if( send(x , &msg , sizeof(msg) , 0) < 0)
			handlerError("Send operation failed.\n");
}

void localSocet(){
	sockUnix = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sockUnix == -1){
        handlerError("Error during creating local socket \n");
			}

	struct sockaddr_un sockadd;
	sockadd.sun_family = AF_UNIX;
  strncpy(sockadd.sun_path, path, 100);

	unlink(path);

if (bind(sockUnix, (struct sockaddr *) &sockadd, sizeof(sockadd)) < 0){
		handlerError("Error during binding 1.\n");
	}

		printf("Binded\n");

}

void netSocket(){
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
        handlerError("Error during creating  not local socket \n");
	struct sockaddr_in sockadd;
	sockadd.sin_family = AF_INET;
	sockadd.sin_addr.s_addr = INADDR_ANY;
	sockadd.sin_port = htons(port);

	if (bind(sockfd , (struct sockaddr *)&sockadd , sizeof(sockadd)) < 0){
		handlerError("Error during binding 2.\n");
	}

		printf("Binded\n");

}
int addClient(int fd){

	if(clientNum >= maxClient){
		printf("there is no free place\n");
		return -1;
	}

		users[clientNum].fd = fd;
		clientNum++;
		return clientNum;
	}


void checkName(char *name, int fd){
	for(int i=0; i<maxClient; i++){
		if((strcmp(name, users[i].name) == 0) && users[i].fd != fd){
			for(int j=i; j<maxClient-1 && users[j].fd != -1; j++){
				users[j].fd = users[j+1].fd;
				users[j].name = users[j+1].name;
			}

			if (close(fd) == -1)
                handlerError("failed closing client fd.\n");

		}
		if(users[i].fd == fd)
		users[i].name = name;
	}
}
void * threadFun(void * arg){

	localSocet();
	netSocket();

	int sd;
	int maxSd;
	int activity;
	int fd;
	for(int i =0; i<maxClient; i++){
		users[i].fd = -1;
	}

	if (listen(sockfd, maxClient) <0)
		handlerError("Error during listening.\n");

	if (listen(sockUnix, maxClient) < 0)
			handlerError("Error during listening.\n");

			FD_ZERO(&readfds);
			FD_SET(sockfd, &readfds);
			FD_SET(sockUnix, &readfds);
			maxSd = sockfd;
	while(1){
		 for ( int i = 0 ; i < maxClient ; i++)
		 {
				 sd = users[i].fd;
				 if(sd != -1)
						 FD_SET( sd , &readfds);
				 if(sd > maxSd)
						 maxSd = sd;
		 }

		 activity = select( maxSd + 1 , &readfds , NULL , NULL , NULL);

		 if(activity == -1) handlerError("Error during select.\n");
			if (FD_ISSET(sockfd, &readfds)){
				struct sockaddr addr;
				socklen_t in_len = sizeof(addr);
				fd = accept(sockfd,&addr, &in_len);
				if (fd == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            break;
                        } else {
                            handlerError("Error during accept.\n");
                            break;
													}
}
				addClient(fd);

			}
			else if( FD_ISSET(sockUnix, &readfds)){
				struct sockaddr addr;
				fd = accept(sockUnix, (struct sockaddr*)&addr, (socklen_t *)sizeof(addr));
				if(fd == -1)
					handlerError("Error during accept.\n");
				addClient(fd);

			}
			else {
				for(int i =0; i< maxClient; i++){
					if(users[i].fd == -1) break;
					else{
						sd = users[i].fd;

						if(FD_ISSET( sd , &readfds)){

							struct message clientMsg;
			        ssize_t rec = recv(sd, &clientMsg, sizeof(clientMsg), MSG_WAITALL);
							if(rec == -1){
								handlerError("Receive message failed.\n");
							}
							else if(rec == 0){
								kill(getpid(), SIGINT);
							}
							switch(clientMsg.type){
									case 6:
										checkName(clientMsg.name, sd);
									break;
									case 4:
										printf("Result: %d\n",clientMsg.arg[0]);
									break;
									case 5:
										printf("Client %d answer\n", sd);
									break;
									default:
										printf("Wrong message.\n");
									break;
							}

						}
					}
				}
		}

	}
	return NULL;

}

int main(int argc , char *argv[])
{
	srand( time( NULL ) );

	signal(SIGINT, handlerSIGINT);
	atexit(exiting);
	if(argc != 3 )
				handlerError("Number of argument is wrong.\n");

		port = atoi(argv[1]);
		path = argv[2];
		if(pthread_create(&thread, NULL, threadFun, NULL) != 0)
			handlerError("Error during creating thread\n");

		th =1;
while(1){
	char * operation = malloc(sizeof(char)*2);
	int a, b;
	int tmp =-1;
	struct message msg;
		printf("Enter an operation and numbers:\n");
		scanf("%s %d %d\n",operation, &a, &b );

		if(strcmp("+", operation) == 0) tmp = 0;
		else  if(strcmp("-", operation) == 0) tmp =1;
		else if(strcmp("*", operation) == 0) tmp = 2;
		else if(strcmp("/", operation) == 0) tmp = 3;
		else printf("Incorrect operation. Try again.\n");

		//
		// switch (tmp) {
		// case 0:
		// 		msg.type = "ADD";
		// 		printf("Jestemr \n");
		// 		break;
		// case 1:
		// 		msg.type = "SUB";
		// 		printf("Jestemr2\n");
		// 		break;
		// case 2:
		// 		msg.type = "MUL";
		// 		printf("Jestemr 3\n");
		// 		break;
		// case 3:
		// 		msg.type = "DIV";
		// 		printf("Jestemr4 \n");
		// 		break;
		// default:
		// 		printf("Incorrect operation. Try again.\n");
		// 		break;
		//
		// }
		if(tmp != -1){
		msg.type =tmp;
		msg.arg[0]=a;
		msg.arg[1] = b;
		counter++;

		sendingMsg(msg);
	}

		free(operation);

}

return 0;

}
