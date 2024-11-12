#include <stdlib.h>
#include <stdio.h>

int main(){
    int allocate;

    printf("Enter MB to allocate: ");

    scanf("%d", &allocate);
    printf("Allocating %d MB of memory...\n", allocate);

    size_t num_elements = (allocate * 1024 * 1024) / sizeof(int);
    int* arr = (int*)malloc(num_elements * sizeof(int));

    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    while(1){
        for (size_t i = 0; i < num_elements; i++){
            printf("%d", arr[i]);
        }
    }

    free(arr);

    return 0;
}