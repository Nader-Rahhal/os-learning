#include <stdlib.h>
#include <stdio.h>

int main(){

    int* arr = malloc(10 * sizeof(int));

    for (size_t i = 0; i < 10; i++){
        arr[i] = i;
    }

    // free(arr);

    return 0;
}