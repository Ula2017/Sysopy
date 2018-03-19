#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include<string.h>

char* formatdate(char* str, time_t val)
{
        strftime(str, 30, "%d.%m.%Y %H:%M:%S", localtime(&val));
        return str;
}
void fileInformation(char* name)
{
    char date[30];
    char buf[PATH_MAX + 1];

	struct stat fileStat;
    if(stat(name,&fileStat) < 0){
     printf("Wrong path.\n");
     exit(1);
     }

    printf("INFORMATION: \n");
    char * real=realpath(name, buf);
    printf("Absolute path is: %s.\n", real);
    printf("Size: \t\t%d bytes. \n",(int)fileStat.st_size);
    printf("File Permissions: \t");
    printf( (fileStat.st_mode & S_IRUSR) ? "-r" : "--");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");
    printf("Last modification: %s\n \n", formatdate(date, fileStat.st_mtime));
}

void searchDir(char * pathName, int size){

    char res[200];
    char fileres[200];
    struct dirent * file;
    struct stat tmp;

    DIR * handler=opendir(pathName);


    if(handler==NULL){
        printf("Error during opening\n");
        exit(1);
    }
    while((file=readdir(handler))!=NULL){

    if(file->d_type==DT_DIR){
        char *dirName = file->d_name;

        if(strcmp(dirName, ".." ) !=0 && strcmp(dirName, ".")!=0){

        strcpy(res, pathName);
        char *result =strcat(pathName, "/");
        result=strcat(pathName,dirName);
        searchDir(result,size);
       strcpy(result,res);
        }
    }
    if(file->d_type==DT_REG ){
         strcpy(fileres, pathName);
        char *filepath =strcat(pathName, "/");
        filepath=strcat(pathName,file->d_name);

        if(stat(filepath,&tmp) < 0){
        printf("Wrong path.\n");
        exit(1);
        }
        if((int)tmp.st_size <= size){
        fileInformation(filepath);
        }
        strcpy(filepath, fileres);
    }
    }
    closedir( handler );


}
//1 sciezka bezwzgledna do katalogu, 2-rozmiar plikow szukanych
int main(int argc, char *argv[]){

    if(argc != 3) {
        printf("Wrong number of arguments. \n ");
        exit(1);
    }
    int size=atoi(argv[2]);
    if(size<=0){
    	printf("Wrong argument number 2. Argument should be numbers. \n");
        exit(1);
        }

        searchDir(argv[1],size);

}
