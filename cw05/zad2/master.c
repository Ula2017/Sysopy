#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char* argv[]){


    char* pipeName;

    if(argc!=3){
        printf("Wrong number of arguments.\n");
        exit(1);
        }

    int R = atoi(argv[2]);

    if(R<=0){
        printf("Size of array is wrong\n");
        exit(1);
    }

    int **T = (int**)calloc(R,sizeof(int*));
    for(int i=0; i<R; i++) T[i]=(int*)calloc(R,sizeof(int));

    pipeName=argv[1];

    if(mkfifo(pipeName,0666) == -1){
        printf("Error durring creating named pipe.\n");
        exit(1);
    }

    //tu tworze dzieci
    char *prog[4]={"./slave",pipeName, "100", "100"};

    for(int i=0; i<50; i++){
        pid_t pid = fork();
        if(pid ==0){
            if(execvp(prog[0],prog) == -1){
                printf("Error during execvp. \n");
                exit(1);
            }
            exit(0);
        }
        else if(pid > 0){
            sleep(5);

        }
        else{
            printf("Error during create PID. \n");
            exit(1);
        }
    }



    FILE * fp = fopen(pipeName, "r");
 //   FILE * fp = fopen("file.txt", "r");

     if (fp == NULL){
        printf("Error during opening pipe.\n");
        exit(1);
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int x,y,iters;
    double re, im;

    while ((read = getline(&line, &len, fp)) != -1) {

        sscanf(strtok(line, " "), "%lf", &re);
        sscanf(strtok(NULL, " "), "%lf", &im);
        iters =atoi(strtok(NULL, " \n"));

        //re=(2+re)/3.0;

        x = (int)((2.0+re)*R/3.0);
        y= (int)((1.0+im)*R/2.0);

        T[x][y]=iters;
        printf("T %d, %d,%d \n", x, y, T[x][y]);
    }

    int datafile=open("data",O_CREAT | O_APPEND | O_WRONLY, S_IWUSR | S_IRUSR );
    if(datafile == -1){
        printf("Error during opening data.\n");
        exit(1);
    }

    for(int i=0; i<R; i++){
        for(int j=0; j<R; j++){

            sprintf(line, "%d %d %d\n", i, j, T[i][j]);
            if(write(datafile,line,strlen(line)) < 0){
                printf("Error during writing to data file.\n");
                exit(1);
            }
       }
    }

    close(datafile);
    //uruchomienie programu gnuplot
    FILE * gnuplot = popen ("gnuplot", "w");
    if (gnuplot == NULL){
        printf("Error during opening gnuplot.\n");
        exit(1);
    }

    fprintf(gnuplot,"set view map\n");
    fprintf(gnuplot,"set xrange [0:%d]\n",R);
    fprintf(gnuplot,"set yrange [0:%d]\n",R);
    fprintf(gnuplot,"plot 'data' with image\n");


    fflush(gnuplot);
    getchar();


    fclose(fp);
    pclose(gnuplot);
    for(int i=0; i<R; i++) free(T[i]);
    free(T);
    if (line) free(line);
    return 0;


}
