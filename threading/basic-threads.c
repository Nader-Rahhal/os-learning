#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

int start = 1;


// THIS WONT PRODUCE DETERMINISTIC (or correct) RESULTS
void *update(void *arg){
    pthread_t tid = pthread_self();
    printf("Running thread: %lu\n", (unsigned long)tid);
    start++;
    return NULL;
}

int main(int argc, char *argv[]){

    if (argc != 2){
        printf("Adjust argument count\n");
        exit(-1);
    }

    int NUM_THREADS = atoi(argv[1]);
    pthread_t threads[NUM_THREADS];

    printf("Start: %d\n", start);
    int expected = start * NUM_THREADS;
    printf("Expected Ending: %d\n", expected);

    for (int i = 0; i < NUM_THREADS; i++){
        
        int rc = pthread_create(&threads[i], NULL, update, NULL);
        if (rc){
            printf("Error creating thread %d\n", i);
            exit(-1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++){
        pthread_join(&threads[i], NULL);
    }

    printf("Actual Ending: %d\n", start);

    return 0;
}