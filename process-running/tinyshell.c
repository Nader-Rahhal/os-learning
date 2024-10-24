#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define BUFFER 1024


int main(){

    while (1){

        char line[100];

        printf("tinyshell> ");

        if(!fgets(line, BUFFER, stdin)) break;

        char *p = strchr(line, '\n');
        if (p) *p = 0;
        if (strcmp(line, "exit") == 0) break;
        char *args[] = {line, (char*)0};
        int rc = fork();

        if (rc == 0){
            execvp(line, args);
            exit(0);
        }

        else {
            wait(NULL);
        }
    
    }
    return 0;
}