#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

void helloGoodbyeProcessAPI(){

    int rc = fork();

    if (rc < 0) printf("Fork failed!\n");
    else if (rc > 0) printf("hello\n");
    else  printf("goodbye\n");

}

void outputBinContents(){

    int rc = fork();
    if (rc < 0) printf("Fork failed!\n");

    else if (rc == 0){
        char *myagrs[3];
        myagrs[0] = strdup("/bin/ls");
        myagrs[1] = strdup("-l");
        myagrs[2] = NULL;
        execvp(myagrs[0], myagrs);
    }

    else {
        int wait_rc = wait(NULL);
        printf("Finished file output!\n");
    }


}

// runs parent and child completely seperate if you next wait() within child process
void inverseWaitUsage(){

    int rc = fork();
    if (rc < 0) printf("Fork failed!\n");

    else if (rc == 0){
        int wait_rc = wait(NULL);
        char *myagrs[3];
        myagrs[0] = strdup("/bin/ls");
        myagrs[1] = strdup("-l");
        myagrs[2] = NULL;
        execvp(myagrs[0], myagrs);
    }

    else {
        printf("Finished file output!\n");
    }


}


void pipeTwoChildProcesses() {
    int rc1 = fork();
    
    if (rc1 < 0) {
        fprintf(stderr, "First fork failed\n");
        exit(1);
    } 
    else if (rc1 == 0) {
        printf("First child (pid: %d)\n", (int)getpid());
        char *myargs[3];
        myargs[0] = "ls";
        myargs[1] = "-l";
        myargs[2] = NULL;
        execvp(myargs[0], myargs);
    } 
    else {
        int rc2 = fork();
        
        if (rc2 < 0) {
            fprintf(stderr, "Second fork failed\n");
            exit(1);
        }
        else if (rc2 == 0) {
            printf("Second child (pid: %d)\n", (int)getpid());
            char *myargs[3];
            myargs[0] = "pwd";
            myargs[1] = NULL;
            execvp(myargs[0], myargs);
        }
        else {
            wait(NULL);  // Wait for first child
            wait(NULL);  // Wait for second child
            printf("Parent: both children completed\n");
        }
    }
}

int main(int argc, char *argv[]){

    inverseWaitUsage();


    return 0;

}