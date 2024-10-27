#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define BUFFER 1024
#define MAX_ARGS 12

int main(){

    while (1){

        char line[100];

        char *greetings[] = {
            "Hello, Mahdi! What shall we embark on today?\n",
            "Oh Moderator! Is that you?\nI was... uh ... busy.\nKnock next time before you come in please!\nAnyways what shall we do today?\n",
            "Jolly day! What is the move on this fine evening?\n",
            ""
        };


        if(!fgets(line, BUFFER, stdin)) break;

        char *p = strchr(line, '\n');
        if (p) *p = 0;
        if (strcmp(line, "exit") == 0) break;
        printf("Command: (%s)\n", line);


        char *args[MAX_ARGS];
        int arg_count = 0;
        
        char *token = strtok(line, " ");
        while (token != NULL && arg_count < MAX_ARGS - 1) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }

        args[arg_count] = NULL;

        int rc = fork();

        if (rc == 0){
            execvp(args[0], args);
            exit(0);
        }

        else {
            wait(NULL);
        }
    
    }
    return 0;
}