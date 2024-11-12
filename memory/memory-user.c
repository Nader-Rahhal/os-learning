#include <stdlib.h>
#include <stdio.h>

int main(){
    int allocate;

    printf("Enter MB to allocate: ");

    scanf("%d", &allocate);
    printf("Allocating %d MB of memory...\n", allocate);

    size_t total_bytes = (size_t)allocate * 1024 * 1024;
    size_t num_elements = total_bytes / sizeof(int);

    int* arr = (int*)malloc(num_elements * sizeof(int));

    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Total allocated: %zu bytes (%zu elements)\n", total_bytes, num_elements);

    for(int i = 0; i < 1000000; i++) {
        arr[i] = i;
    }

    printf("Bytes actually touched: %zu\n", 1000000 * sizeof(int));

    free(arr);

    return 0;
}