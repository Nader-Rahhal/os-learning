#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void basicProcessAPI(){

    printf("Runing parent process (%d)\n", (int) getpid());
    int x = 10;
    int rc = fork();

    if (rc < 0) printf("Fork failed!\n");

    else if (rc == 0){
        printf("Running child process (%d)\n", (int) getpid());
        x--;
        printf("(%d) --> x: (%d)\n", getpid(), x);
    }

    else {
        int rc_wait = wait(NULL); // Make parent process wait on child process to complete
        x++;
        printf("(%d) --> x: (%d)\n", getpid(), x);
    }
}

void concurrentFileEditing(){
    
}

int main(int argc, char *argv[]){

    basicProcessAPI();


    return 0;

}