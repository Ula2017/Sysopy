#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/times.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
//0 - nazwa programu 1- sys/lib 2-funkcja 3- nazwa pliku 4 - ilosc rekordow 5 wielkosc
void generate(char *fileName, int size, int recordNum){

    char* buf = calloc (size, recordNum);
	int n = size * recordNum;

	int fileDev = open("/dev/urandom", O_RDONLY);
	if(fileDev == -1) exit(1);
	int result = open(fileName,O_CREAT | O_RDWR | O_APPEND | O_TRUNC, S_IRUSR | S_IWUSR |S_IRGRP | S_IWGRP);
	if(result == -1) exit(1);

	int b=read(fileDev,buf,n);
	if(b != n){
	printf("Error during reading /dec\v/urandom");
	free(buf);
	exit(1);
	}

	b = write(result, buf, n);
    if(b != n){
	printf("Error during writing. \n");
	free(buf);
	exit(1);
	}

	if(close(fileDev) == -1 || close(result) == -1){
	printf("Error during closing file. \n");
	free(buf);
	exit(1);
	}
	free(buf);

}

int readSys(char* file,char* a,int size, int position)
{

	int filedesc = open(file, O_RDWR);
	if(lseek(filedesc,position,SEEK_SET) < 0) exit(1);
	if(read(filedesc,a,size) < 0)
     	{
	  printf("Error during reading");
	  exit(1);
	}

    close(filedesc);
	return 0;
}
int writeSys(char* file,char* a,int size, int position)
{

	int filedesc = open(file, O_RDWR);
    if(lseek(filedesc,position,SEEK_SET) < 0) exit(1);
	if(write(filedesc,a,size) < 0)
     	{
	  printf("Error during writing");
	  exit(1);
	}

	close(filedesc);
	return 0;
}
void shuffleSys(char* file, int n, int size)
{

	for(int i = n-1;i>0;i--)
	{
	  char* a = calloc(1,size);
	  char* b = calloc(1,size);
	  int j = rand()%i;
	  readSys(file,a,size,i*size);
	  readSys(file,b,size,j*size);
 	  writeSys(file,a,size,j*size);
	  writeSys(file,b,size,i*size);

	  free(a);
	  free(b);

	}

}
void bubbleSortSys( char *file, int recordNum, int size)
{
    char *a=malloc(size);
    char *b=malloc(size);

    for( int i = 0; i < recordNum; i++ )
    {
        for( int j = 0; j < recordNum- 1; j++ )
        {

            readSys(file,a,size,i*size);
            readSys(file,b,size,j*size);
            if(strncmp(a,b,1) < 0){
                 writeSys(file,b,size,i*size);
                 writeSys(file,a,size,j*size);
                 }
        }
    }
    free(a);
    free(b);
}

FILE * readLib(char *file, char * a, int size, int position){

    FILE * filedesc=fopen(file,"r+");
    fseek(filedesc,position,SEEK_SET);

    if(fread(a,1,size,filedesc)!=size){
    printf("Error during reading \n");
    exit(1);
    }
     fflush(filedesc);
    if(fclose(filedesc)!=0){
    printf("Error during closing file1. \n");
    exit(1);
    }
    return filedesc;
}
FILE *writeLib(char *file, char * a, int size, int position){

    FILE * filedesc=fopen(file,"r+");
    fseek(filedesc,position,SEEK_SET);

    if(fwrite(a,1,size,filedesc)!=size){
    printf("Error during reading \n");
    exit(1);
    }
    fflush(filedesc);
    if(fclose(filedesc)!=0){
    printf("Error during closing file. \n");
    exit(1);
    }
    return filedesc;

}

void shuffleLib(char *file, int recordNum, int size){

    for(int i =recordNum-1; i>0; i--){
        char* a = calloc(1,size);
        char* b = calloc(1,size);
        int j = rand()%i;
        readLib(file,a,size,i*size);
        readLib(file,b,size, j*size);
        writeLib(file,b,size,i*size);
        writeLib(file,a,size,j*size);
        free(a);
        free(b);
        }
}

void bubbleSortLib(char * file, int recordNum, int size){


        char *a=malloc(size);
    char *b=malloc(size);

    for( int i = 0; i < recordNum; i++ )
    {
        for( int j = 0; j < recordNum- 1; j++ )
        {

            readLib(file,a,size,i*size);
            readLib(file,b,size,j*size);
            if(strncmp(a,b,1) < 0){
                writeLib(file,b,size,i*size);
                writeLib(file, a,size,j*size);
            }
        }

    }
    free(a);
    free(b);



    }

double usertime(struct tms* start, struct tms* stop){
return (double)((stop->tms_utime)-(start->tms_utime))/(double)sysconf(_SC_CLK_TCK);
}
double systime(struct tms* start, struct tms* stop){
return (double)(stop->tms_stime-start->tms_stime)/(double)sysconf(_SC_CLK_TCK);
}
void generateRaport(struct tms *start, struct tms *stop, int size, int recordNum, char * kind, char *operation){

     printf("Time of %s operation %s. Record number: %d, size of record: %d. \n",operation,kind,recordNum,size);
	 printf("usertime: %.10f, systime: %.10f \n",usertime(start,stop),systime(start,stop));

	}
int main(int argc, char *argv[])
{
    srand(time(NULL));
    int tmpS;
    int tmpF;
    struct tms *start;
    struct tms *stop;
    start=malloc(sizeof(struct tms));
    stop=malloc(sizeof(struct tms));

    if(argc != 6) {
    printf("Wrong number of arguments. \n ");
    exit(1);
    }

	int size = atoi(argv[4]);
	int recordNum = atoi(argv[5]);

	if(size <= 0 || recordNum <=0) {
	printf("Wrong argument 5 or 6. Arguments 5 and 6 should be numbers. \n");
	exit(1);
	}
    if(strcmp(argv[1], "sys")==0) tmpS =1;
    else if(strcmp(argv[1], "lib")==0) tmpS =2;
    else tmpS=0;

    if(strcmp(argv[2],"generate")==0) tmpF=1;
    else if(strcmp(argv[2],"shuffle")==0) tmpF=2;
    else if(strcmp(argv[2],"sort")==0) tmpF=3;
    else tmpF=0;

	switch(tmpS){
	case 1:
        switch(tmpF){
        case 1:
        times(start);
        generate(argv[3],size, recordNum);
        times(stop);
        generateRaport(start,stop,recordNum,size, "generate", "sys");
        break;
        case 2:
        times(start);
        shuffleSys(argv[3],recordNum,size);
        times(stop);
        generateRaport(start,stop,recordNum,size, "shuffle", "sys");
        break;
        case 3:
        times(start);
        bubbleSortSys(argv[3],recordNum,size);
        times(stop);
        generateRaport(start,stop,recordNum,size, "sort", "sys");

        break;
        default:
            printf("Argument 4 is wrong. \n");
            exit(1);
            }
        break;
    case 2:
        switch(tmpF){
        case 1:
        times(start);
        generate(argv[3],size, recordNum);
        times(stop);
        generateRaport(start,stop,recordNum,size, "generate", "sys");
        break;
        case 2:
        times(start);
        shuffleLib(argv[3],recordNum,size);
        times(stop);
        generateRaport(start,stop,recordNum,size, "shuffle", "lib");
        break;
        case 3:
        times(start);
        bubbleSortLib(argv[3],recordNum,size);
        times(stop);
        generateRaport(start,stop,recordNum,size, "sort", "lib");
        break;
        default:
            printf("Argument 4 is wrong. \n");
            exit(1);
            }
        break;
    default:
        if(tmpF==1) generate(argv[3],size, recordNum);
        else{
        printf("Second argument is uncorrect. \n");
        exit(1);
        }
        }


        free(stop);
        free(start);


//    generate(argv[3], size, recordNum);
//    shuffleLib("buble.txt",recordNum,size);
//   bubbleSortLib("buble.txt",10,2);

   return 0;
}
