#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

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


void childPiping() {

    int pipefd[2];
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }
    
    int rc1 = fork();
    
    if (rc1 == 0) {
        close(pipefd[0]);                   
        dup2(pipefd[1], STDOUT_FILENO);      
        close(pipefd[1]);
        
        execlp("ls", "ls", "-l", NULL);      
        perror("execlp ls");
        exit(1);
    }
    
    int rc2 = fork();
    
    if (rc2 == 0) {
        close(pipefd[1]);                  
        dup2(pipefd[0], STDIN_FILENO);       
        close(pipefd[0]);
    
        execlp("grep", "grep", "f", NULL);
        perror("execlp grep");
        exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    
    wait(NULL);
    wait(NULL);
}


int main(int argc, char *argv[]){

    childPiping();


    return 0;

}