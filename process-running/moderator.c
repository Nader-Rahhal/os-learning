#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define BUFFER 1024
#define MAX_ARGS 12

char *greetings[] = {
            "Salutations, Mahdi!?\n",
            "Jolly day Mods! Hope yours is also jolly.\n",
            "RoOoOoOo!!!!\n"
};

char *jokes[] = {
    "The Al Aqsa to pervert pipeline has been allgedly growing in recent years I've heard...\n",
    "Never forget the great tragedy of Ghareebathon 2024, where an innocent mango lasi was britually ran through nonconsentually through the mouths of thousands...\n",
    "The Armenian 'Genocide'\n",
    "The Armenian 'Genocide'\n", // one with the link to the generator
    "Did you know in Hand if you are 45 degrees North of the Dealer and a nationality that borders the nationality of the player to your righthand side during March of a the leap year, you can discard an extra card?\n",
    "Wait, stop omg. You're lowkey eating rn\n",
    "An emotional and gentle man.\n",
    "Me when the Andersonville Village Discount aware (THOSE WHO KNOW)!\n",
    "Malik did not die for this...\n",
    "Strongest Kabobi rotten tomato enjoyer when they meet the weakest Black Bean Burger enthusiast\n",
    "Omg Modi!!! I loved that tabooley of urs smmmmmmmmmmmmmm!!\n",
    "Hyped for the next JanFanCam x Moderationator collab\n",
    "That god awful photo taken at Ghareeb. You know the one I am referring to.\n",
    "Gooners genuinly do anything for the huzz...\n",
    "You're basically a POC in comparison to Nader if I am being honest.\n",
    "'Mahdi you're sooo big bro coded. Have I told you that before\n-- Aala'\n",
    "I swear to god no one ever fucked me so brutal that they pinned me down.\n",
    "Did you just call yourself falastini?..\n",
    "Average pantaloons wearer.\n",
    "One day Mahdi, you will find someone who will take care of you the way Nader takes cares of his socks.\n",
    "It is on nights like these that you have to remember the RoOoOoOo is always there for you.\n",
};

int main(){

    printf("%s", greetings[0]);
    printf("I am a multipurpose shell - I can run commands, tell jokes, and much more!\n");

    while (1){

        printf("> ");

        char line[100];


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

        if (strcmp(line,"menu") == 0){
            printf("Custom Commands:\n");
            printf("lol - runs joke simulator\n");
            printf("Please contact rahhalnader04@gmail.com with any thoughts.\n");
        }

        else {

            int rc = fork();

            if (rc == 0){
                execvp(args[0], args);
                exit(0);
            }

            else {
                wait(NULL);
            }
        }


    
    }
    return 0;
}