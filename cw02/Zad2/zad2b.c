#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <limits.h>
int size;

char* formatdate(char* str, time_t val)
{
        strftime(str, 30, "%d.%m.%Y %H:%M:%S", localtime(&val));
        return str;
}

static int display_info(const char *fpath, const struct stat *sb,
             int tflag, struct FTW *ftwbuf)
{
    if(S_ISDIR(sb->st_mode)) return 0;
    if(sb->st_size >size) return 0;
    char date[30];
    char buf[PATH_MAX + 1];


    printf("INFORMATION: \n");
    char * real=realpath(fpath, buf);
    printf("Absolute path is: %s.\n", real);
    printf("Size: \t\t%d bytes. \n",(int)sb->st_size);
    printf("File Permissions: \t");

    printf( (sb->st_mode & S_IRUSR) ? "-r" : "--");
    printf( (sb->st_mode & S_IWUSR) ? "w" : "-");
    printf( (sb->st_mode & S_IXUSR) ? "x" : "-");
    printf( (sb->st_mode & S_IRGRP) ? "r" : "-");
    printf( (sb->st_mode & S_IWGRP) ? "w" : "-");
    printf( (sb->st_mode & S_IXGRP) ? "x" : "-");
    printf( (sb->st_mode & S_IROTH) ? "r" : "-");
    printf( (sb->st_mode & S_IWOTH) ? "w" : "-");
    printf( (sb->st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");
    printf("Last modification: %s\n \n", formatdate(date, sb->st_mtime));

    return 0;           /* To tell nftw() to continue */
}

int
main(int argc, char *argv[])
{
    if(argc != 3) {
        printf("Wrong number of arguments. \n ");
        exit(1);
    }
    size=atoi(argv[2]);
    if(size<=0){
    	printf("Wrong argument number 2. Argument should be numbers. \n");
        exit(1);
        }

    int flags= FTW_DEPTH | FTW_PHYS;

   if (nftw(argv[1], display_info, 20, flags)
            == -1) {
        printf("Wrong path.\n");
        exit(1);
    }
    exit(EXIT_SUCCESS);
}
