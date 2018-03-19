#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/types.h>

int sockfd;
struct message{
	int type;
	int arg[2];
	char name[100];
};


void handlerError(char * message){
    printf(message);
		if (shutdown(sockfd, SHUT_RDWR) == -1)
				printf("Error during shutting down socket");
		if (close(sockfd) == -1)
				printf("Error during closing socket");
		exit(1);
}

void handlerSIGINT(int x){

        if (shutdown(sockfd, SHUT_RDWR) == -1) {
            puts("Error during shutting down socket");
        }

        if (close(sockfd) == -1) {
            puts("Error during closing socket");
        }
				exit(0);

}
void localSocet(char * addr){
	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sockfd == -1){
        handlerError("Error during creating local socket \n");
			}

	struct sockaddr_un sockadd;
	sockadd.sun_family = AF_UNIX;
  strncpy(sockadd.sun_path, addr, 100);

	if (connect(sockfd , (struct sockaddr *)&sockadd , sizeof(sockadd)) < 0)
				handlerError("Connect failed. Error");

		puts("Connected\n");

}

void netSocket(char * addr, in_port_t port){
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1)
        handlerError("Error during creating  not local socket \n");
	struct sockaddr_in sockadd;
	sockadd.sin_family = AF_INET;
	sockadd.sin_port = htons(port);

	if(inet_aton(addr, &sockadd.sin_addr) == 0)
		handlerError("Incorrect IP Address.\n");

	if (connect(sockfd , (struct sockaddr *)&sockadd , sizeof(sockadd)) < 0)
				handlerError("Connect failed. Error");

		puts("Connected\n");

}
void sendingMsg(struct message msg){
	if( send(sockfd , &msg , sizeof(msg) , 0) < 0)
			handlerError("Send message failed.\n");
}
//nazwa, 1/2-local/nie local
int main(int argc , char *argv[])
{

	signal(SIGINT, handlerSIGINT);
  char * clientName = malloc(sizeof(char)*100);
  clientName = argv[1];
  int type = atoi(argv[2]);
  if(type != 1 && type !=2){
      handlerError("Second argument should be 1 or 2.\n");

		}
	if(type == 1 && argc != 4 )
			handlerError("Number of argument is wrong.\n");
	if(type == 2 && argc != 5 )
				handlerError("Number of argument is wrong.\n");

		char *serverAddr= malloc(sizeof(char)*100);
		serverAddr = argv[3];
		in_port_t port;
		if(type == 2)
			port = (in_port_t)atoi(argv[4]);

		if(type == 1){
			localSocet(serverAddr);
		}else{
			netSocket(serverAddr, port);
		}

		struct message msg;
		msg.type = 6;
		strncpy(msg.name, clientName, 100);

		if( send(sockfd , &msg , sizeof(msg) , 0) < 0)
				handlerError("Send name failed");

    while(1)
    {
				struct message serverMsg;
        ssize_t rec = recv(sockfd, &serverMsg, sizeof(serverMsg), MSG_WAITALL);
				if(rec == -1)
					handlerError("Receive message failed.\n");
				else if(rec == 0)
					kill(getpid(), SIGINT);

				struct message reply;
				reply.type = 4;
				switch(serverMsg.type){
					case 0:
						reply.arg[0]=serverMsg.arg[0]+ serverMsg.arg[1];
						reply.arg[1] = 0;
						sendingMsg(reply);
					break;
					case 1:
						reply.arg[0]=serverMsg.arg[0]- serverMsg.arg[1];
						reply.arg[1] = 0;
						sendingMsg(reply);
					break;
					case 2:
						reply.arg[0]=serverMsg.arg[0]* serverMsg.arg[1];
						reply.arg[1] = 0;
						sendingMsg(reply);
					break;
					case 3:
						if(serverMsg.arg[1] == 0)
							reply.arg[1] = -1;
						else{
						reply.arg[0]=serverMsg.arg[0]/ serverMsg.arg[1];
						reply.arg[1] = 0;
						}
						sendingMsg(reply);
					break;
					case 4:
						reply.type = 5;
						sendingMsg(reply);
					break;
					default:
						printf("I got wrong message.\n");
					break;
		}

				}


		free(clientName);
		free(serverAddr);

    return 0;
}
