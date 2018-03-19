#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<signal.h>
#include<string.h>

int counter =0;

void handlerSIGINT(int signal){

    puts("The child is closed.\n");
    exit(1);
}

void handlerSIGUSR1(int signal){

    counter++;
    kill(getppid(), SIGUSR1);
}

void handlerSIGUSR2(int signal){

	puts("I got signal SIGUSR2 from my parent.\n");
	printf("The child %d received %d SIGUSR1.\n",(int)getpid(), counter);
    exit(0);
}

void handlerSIGREAL1(int signal){

    counter++;
    kill(getppid(), SIGRTMIN+3);
}

void handlerSIGREAL2(int signal){

	puts("I got SIGREAL. Closing.\n");
	printf("The child %d received %d SIGUSR1.\n",(int)getpid(), counter);
    exit(0);
}



int main(int argc, char* argv[]){


    signal(SIGINT, &handlerSIGINT);
	signal(SIGUSR1,&handlerSIGUSR1);
	signal(SIGUSR2,&handlerSIGUSR2);
	signal(SIGRTMIN+3, &handlerSIGREAL1);
	signal(SIGRTMIN+10, &handlerSIGREAL2);

    while(1) pause();

return 0;

}

