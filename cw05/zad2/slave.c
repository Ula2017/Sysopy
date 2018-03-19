#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <complex.h>

int generateIters(double re, double im, int K){

  double complex c=re + I*im;
  double complex z=0.0;
  int iters=1;

  for(iters=1;iters<K && cabs(z)<2.0;iters++){
    z=cpow(z,2.0)+c;
  }

  return iters;


}
int main(int argc, char* argv[]){

    srand(time(NULL));

    if(argc!=4){
        printf("Wrong number of arguments.\n");
        exit(1);
        }
    int N=atoi(argv[2]);
    int K=atoi(argv[3]);
    char * pipePath =argv[1];

    if(N<=0 || K<=0){
        printf("Argument N or K is wrong.\n");
        exit(1);
    }

    int filedesc=open(pipePath, O_WRONLY | O_APPEND);
    if(filedesc == -1){
        printf("Error during opening pipe.\n");
        exit(1);
    }
    char*line  = calloc(1,20);


    for(int i=0; i<N; i++){

        double re = -2+((double)rand()/(double)RAND_MAX)*3;
        double im = (-1+((double)rand()/(double)RAND_MAX)*2);

        int iters = generateIters(re, im, K);

        sprintf(line, "%f %f %d\n", re, im, iters);


        if(write(filedesc,line,strlen(line)) < 0){
            printf("Error during writing");
            exit(1);
       }

    }

    free(line);
    close(filedesc);
    return 0;


}
