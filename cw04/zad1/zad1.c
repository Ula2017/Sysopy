#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<signal.h>
#include<string.h>

int n=0;

void handlerSIGTSTP(int x){

    puts("\nI got SIGTSTP. \n");
    if( n == 0 )n=1;
    else n=0;


}

void handlerSIGINT(int y){
    puts("\nI got SIGINT. Closing.\n");
    exit(0);
}
int main(){


    char sign = 'a';
    struct sigaction sa;
    sa.sa_handler = handlerSIGTSTP;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    struct sigaction it;
    it.sa_handler = handlerSIGINT;
    it.sa_flags = 0;
    sigemptyset(&it.sa_mask);


    if ( sigaction(SIGTSTP, &sa, NULL) == -1 ) {
        perror("Couldn't set SIGTSTP handler");
        exit(1);
    }

    signal(2,handlerSIGINT);

   while(1){

        while((int)sign <= 122 && n == 0){
        printf("%c \n", sign );
        fflush(stdout);
        sign+=1;
        sleep(2);
        if(sign == 123) sign = 97;

        }

        while((int)sign >= 97 && n == 1){
            printf("%c \n", sign );
            fflush(stdout);
            sign-=1;
            sleep(2);
            if(sign == 96) sign = 122;

        }
        }


}


