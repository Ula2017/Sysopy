#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


//1-nazwa pliku z poleceniami wsadowymi

//pojawila sie zmienna srodowiskowa
void workEnv( char * line){

    char * name = strtok(line+1, " \t\r\n");
    char * value =strtok(NULL, " \t\r\n");
    if(value == NULL){
        int result = unsetenv(name);
        if(result == -1){
        printf("Removing the environment variable failed. \n");
        exit(1);
        }
    }
    else{
        int set =setenv(name, value, 0); //1 bo jak zmienna istnieje to ma ja nadpisac

        if(set == -1){
        printf("Setting the environment variable failed. \n");
        exit(1);
        }

    }

}

//pojawilo sie polecenie wiec pid
void execute(char * line){
    char * name = strtok(line, " \t\n\r");
    char * tmp;

    int size =1;
    char * argv[10];
    int status;
    for(int i=0; i<10; i++){
    argv[i]=malloc(10*sizeof(char*));
    argv[i]=NULL;
    }
    argv[0]=name;
    while((tmp=strtok(NULL," \t\n\r")) != NULL && size<10){
        argv[size]=tmp;
        size=size+1;
    }
    if(size==11){
    printf("The maximum number of arguments exceeded. \n");
    exit(1);
    }

    pid_t pid=fork();
    if(pid == 0){
        if(execvp(argv[0],argv) == -1){
            printf("Error during execvp. \n");
            exit(1);
        }
    }
    else if(pid > 0){
        wait(&status);
        if(status != 0){
               printf("Status is wrong. \n ");
               exit(1);
        }
    //rodzic -czekamy
    }
    else{
        printf("Error during create PID. \n");
        exit(1);
    }



}

int main(int argc, char* argv[]){

        if(argc !=2){
        printf("Wrong number of arguments.");
        exit(1);
        }

        FILE * f = fopen(argv[1], "r");
        if(f == NULL){
        perror("Opening file failed" );
        exit(1);
        }

        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        //read - ilosc przeczytanych znakow
        while( (read=getline(&line, &len, f)) != -1){


            if(read>1){
                if(line[0] == '#') workEnv(line);
                else execute(line);

            }
        }

        free(line);
        fclose(f);
        return 0;


}
