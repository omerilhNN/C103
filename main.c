#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

void* routine(void* arg) {
    int index = *(int*)arg;
    int sum = 0;
    for (int j = 0; j < 5; j++) {
        sum += primes[index + j];
    }
    printf("Local sum: %d\n", sum);
    *(int*)arg = sum;
    return arg;
}
void* routine1(void* arg) {
    pthread_t tid = *(pthread_t*)arg;
    printf("Hello: Routine thread ID: %ld\n", tid);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    pthread_t thSum[2], thPrint[2];
    int i;
    for (i = 0; i < 2; i++) {
        int* a = malloc(sizeof(int));
        *a = i * 5;
        if (pthread_create(&thSum[i], NULL, &routine, a) != 0) {
            perror("Failed to create threadSum\n");
        }
        if (pthread_create(&thPrint[i], NULL, routine1, &thSum[i]) != 0) {
            perror("Failed to create ROUTINE1\n");
        }
    }
    int globalSum = 0;
    for (i = 0; i < 2; i++) {
        int* r;
        if (pthread_join(thSum[i], (void**)&r) != 0) {
            perror("Failed to join thread\n");
        }
        if (pthread_join(thPrint[i], NULL)){
            perror("Failed to join print thread\n");
        }
        globalSum += *r;
        free(r);
    }
    printf("Global sum: %d\n", globalSum);
    return 0;
}