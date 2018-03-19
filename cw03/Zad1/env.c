#include <stdio.h>
#include <stdlib.h>

//1-nazwa pliku z poleceniami wsadowymi

int main(int argc, char* argv[]){

    if(argc !=2){
        printf("Wrong number of arguments.");
        exit(1);
        }

    char * envp = getenv(argv[1]);

    if(envp == NULL){
    printf("Environment variable %s isn't set. \n", argv[1]);
    exit(1);
    }

    printf("Environment variable is %s. \n", envp );

    return 0;

}
