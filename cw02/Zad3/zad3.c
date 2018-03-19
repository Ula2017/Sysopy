#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>

int readSys(int filedesc,char* a,int size, int position)
{

     if(lseek(filedesc,position,SEEK_SET) < 0){
     printf("Error during lseek function");
      exit(1);
      }
	if(read(filedesc,a,size) < 0)
     	{
	  printf("Error during reading");
	  exit(1);
	}

	return 0;
}

int writeSys(int filedesc,char* a,int size, int position)
{

    if(lseek(filedesc,position,SEEK_SET) < 0) exit(1);
	if(write(filedesc,a,size) < 0)
     	{
	  printf("Error during reading");
	  exit(1);
	}

	return 0;
}

//readWrite 11-wersja nieblokujaca 1, 12 - wersja 1 blokujaca , analogicznie 21, 22
void lockOnByte(int fd, int byte, int readWrite){

    struct flock *fl=malloc(sizeof(struct flock));

    switch(readWrite){
    case 11: //write nieblokujacy
        printf("pisz nieblok");
        fl->l_type=F_WRLCK;
        fl->l_whence=SEEK_SET;
        fl->l_start=byte;
        fl->l_len=1;
       if(fcntl(fd,F_SETLK,fl)== -1){
            printf("You can't lock this sign.");
            exit(1);
       }
        break;
    case 12: //write blokujacy
        fl->l_type=F_WRLCK;
        fl->l_whence=SEEK_SET;
        fl->l_start=byte;
        fl->l_len=1;
        fcntl(fd,F_SETLKW,fl);

        break;
    case 21: //read nieblokujacy
        printf("wchodze");
        fl->l_type=F_RDLCK;
        fl->l_whence=SEEK_SET;
        fl->l_start=byte;
        fl->l_len=1;
       if( fcntl(fd,F_SETLK,fl)== -1){
            printf("You can't lock this sign.");
            exit(1);
       }
        break;
    case 22: //read blokujacy
        fl->l_type=F_RDLCK;
        fl->l_whence=SEEK_SET;
        fl->l_start=byte;
        fl->l_len=1;
        fcntl(fd,F_SETLKW,fl);
        break;
    }


}

void unlockOnByte(int fd, int byte, int readWrite){

    struct flock *fl=malloc(sizeof(struct flock));

    switch(readWrite){
    case 11: //write nieblokujacy
        fl->l_type=F_UNLCK;
        fl->l_whence=SEEK_SET;
        fl->l_start=byte;
        fl->l_len=1;
       if(fcntl(fd,F_SETLK,fl)== -1){
            printf("You can't unlock this sign.");
            exit(1);
       }
        break;
    case 12: //write blokujacy
        fl->l_type=F_UNLCK;
        fl->l_whence=SEEK_SET;
        fl->l_start=byte;
        fl->l_len=1;
        fcntl(fd,F_SETLKW,fl);

        break;
    case 21: //read nieblokujacy
        fl->l_type=F_UNLCK;
        fl->l_whence=SEEK_SET;
        fl->l_start=byte;
        fl->l_len=1;
       if( fcntl(fd,F_SETLK,fl)== -1){
            printf("You can't unlock this sign.");
            exit(1);
       }
        break;
    case 22: //read blokujacy
        fl->l_type=F_UNLCK;
        fl->l_whence=SEEK_SET;
        fl->l_start=byte;
        fl->l_len=1;
        fcntl(fd,F_SETLKW,fl);
        break;
    }


}

void printLock(struct flock *fl, int position){

    printf("Lock on byte %d, PID number is: %d, Type of lock: %d\n", position,fl->l_pid, fl->l_type);
}

void displayLocks(int fd, int size){
    struct flock *fl=malloc(sizeof(struct flock));
    int result;

    for(int i=0; i<size; i++){
    fl->l_type=F_WRLCK;
    fl->l_whence=SEEK_SET;
    fl->l_len=1;
    fl->l_start=i;
    result=fcntl(fd,F_GETLK,fl);
    if(result == -1){
    printf("Error during checking lock");
    exit(1);
    }
    if(fl->l_type == F_RDLCK || fl->l_type == F_WRLCK)
    printLock(fl,i);
    }

}



int main(int argc, char *argv[]){

    int arg;
    int byte;
    int b;
    char * a=malloc(sizeof(char));
    int end =1;

    int tmp;
    if(argc != 2){
    printf("Wrong number of arguments. \n");
    exit(1);
    }

    int fd = open(argv[1], O_RDWR);

    if(fd == -1){
    printf("Error during opening file. File may not exist. \n");
    exit(1);
    }

    while(end ==1){
    printf("Choose option: \n 1. set sign lock on writing. \n 2. set sign lock on reading. \n ");
    printf("3.display sign locks and process PID number\n 4. release choosen lock. \n ");
    printf("5.read choosen sign. \n 6. overwrite choosen sign. \n");
   scanf("%d", &arg);
   if(arg !=1 && arg !=2 && arg !=3 && arg !=4 && arg !=5 && arg !=6){
        printf("You choose wrong option.");
        exit(1);
   }
   printf("Enter byte number: \n");
   scanf("%d", &byte);

    if(byte<=0){
        printf("Byte number is wrong");
        exit(1);
    }




    int size=lseek(fd,0,SEEK_END);

    if(byte > size){
   printf("The number of byte is bigger than size of file.");
   exit(1);
   }


   if(arg == 1 || arg ==2){
   printf("Enter 1 if you want to blocking version. \nEnter 2 if you don't want blocking verion.\n");
   scanf("%d", &tmp);

    printf("%d", arg);

   switch(tmp){
   case 1:
        if(arg ==1) arg =12;
        if(arg ==2) arg = 22;
        break;
    case 2:
        if(arg ==1) arg =11;
        if(arg ==2) arg = 21;
        break;
    default:
        printf("Wrong parametr option.");
        exit(1);
   }
   }


    switch(arg){
    case 11:
        lockOnByte(fd,byte, arg);
        break;
    case 12:
        lockOnByte(fd,byte, arg);
        break;
    case 21:
        lockOnByte(fd,byte, arg);
        break;
    case 22:
        lockOnByte(fd,byte, arg);
        break;
    case 3:
        displayLocks(fd,size);
        break;
    case 4:
        printf("Enter 12 if you want to unclock blocking version on writing. \n");
        printf("Enter 11 if you want to unclock not blocking version on writing. \n");
        printf("Enter 22 if you want to unclock blocking version on reading. \n");
        printf("Enter 21 if you want to unclock not blocking version on reading. \n");
        scanf("%d", &b);
        unlockOnByte(fd,byte,b);
        break;
    case 5:
        readSys(fd,a,1,byte);
        printf("Readed sign is: %s", a);
        break;
    case 6:
        printf("Enter char which replays the old one: \n");
        scanf("%s",a);
        writeSys(fd, a,1,byte);
        break;
        }

        printf("Do you want to continue? Enter 1 if you want to continue\n");
        scanf("%d", &end);

        }

    close(fd);


}
