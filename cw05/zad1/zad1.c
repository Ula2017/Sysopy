#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void parseLine ( char * buffor){

    char *comend[30][10];
    int z=0;

    for(int k=0; k<30; k++){
        while(z<10){
            comend[k][z]=NULL;
            z++;
        }
        z=0;

    }

    int i=0;
    int j=0;

    char * arg=strtok(buffor, " \n");
    while(arg != NULL){

        if(strcmp(arg, "|") == 0){
            i++;
            j=0;
            arg = strtok(NULL, " \n");

        }
        comend[i][j] = arg;
        arg = strtok(NULL, " \n");
        j++;
        if( j>9 || i>29){
            printf("Too many arguments or commends. \n");
            exit(1);
        }
    }


        int fd[30][2];

        for(int k=0; k<i; k++) pipe(fd[k]);

        for(int k=0; k<=i; k++){

            pid_t pid = fork();
             if (pid == 0)
			 {
                if(k==0)
					{
						close(fd[0][0]);
						dup2(fd[0][1],STDOUT_FILENO);
						close(fd[0][1]);
					}
                else if(k==i)
					{
						close(fd[k-1][1]);
						dup2(fd[k-1][0],STDIN_FILENO);
						close(fd[k-1][0]);
					}
                else{
								close(fd[k-1][1]);
								dup2(fd[k-1][0],STDIN_FILENO);
								close(fd[k-1][0]);

								close(fd[k][0]);
								dup2(fd[k][1],STDOUT_FILENO);
								close(fd[k][1]);

						}
            if(execvp(comend[k][0],comend[k]) == -1){
                printf("Error during execvp. \n");
                exit(1);
            }

				 exit(0);

            }
        else if(pid > 0){
            if(k !=0){
                close(fd[k-1][1]);
                close(fd[k-1][0]);
                }
        }
        else{
            printf("Error during create PID. \n");
            exit(1);
        }

        }






}
int main(int argc, char* argv[]){

    if(argc != 1){
        printf("Wrong number of arguments. \n");
        exit(1);
    }

    size_t len = 100;
    char *line = malloc(sizeof(char));


    while(1){

    sleep(2);
    printf("Do you want to parse line?\n");
    getline(&line, &len,stdin);
    if (strcmp(line, "no") == 10) break;
    else{
    printf("\n");
    getline(&line, &len,stdin);
    parseLine(line);
    }
    }

    return 0;


}
