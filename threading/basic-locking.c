#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

int start = 1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void *update(void *arg){
    pthread_t tid = pthread_self();
    printf("Running thread: %lu\n", (unsigned long)tid);

    pthread_mutex_lock(&lock);
    start++;
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(int argc, char *argv[]){

    if (argc != 2){
        printf("Adjust argument count\n");
        exit(-1);
    }

    int NUM_THREADS = atoi(argv[1]);
    pthread_t *threads = calloc(NUM_THREADS, sizeof(pthread_t));

    printf("Start: %d\n", start);
    int expected = start + NUM_THREADS;
    printf("Expected Ending: %d\n", expected);

    for (int i = 0; i < NUM_THREADS; i++){
        
        int rc = pthread_create(&threads[i], NULL, update, NULL);
        if (rc){
            printf("Error creating thread %d\n", i);
            exit(-1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++){
        void *ret;
        int rc = pthread_join(&threads[i], &ret);
        if (rc){
            fprintf(stderr, "Error joining thread %d: %d\n", i, rc);
            continue;
        }
    }

    printf("Actual Ending: %d\n", start);
    pthread_mutex_destroy(&lock);
    free(threads);
    return 0;
}