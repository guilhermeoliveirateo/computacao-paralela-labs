#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 4
#define VECTOR_SIZE 1000000

long long global_sum = 0;

long long thread_sum(void* arg){
    long id = (long) arg;
    long inicio = id * (VECTOR_SIZE / NUM_THREADS);
    for (int i = start; i < end; i++) {
        sum += vector[i];
    }
    return sum;
}

int main() {
    pthread_t threads[NUM_THREADS];

    long vector[VECTOR_SIZE];

    for (long i = 0; i < VECTOR_SIZE; i++){
        vector[i] = 1;
    }

    for (long i = 0; i < NUM_THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, thread_sum, (void*)i) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    long long expected = (long long) VECTOR_SIZE;
    printf("\n--- Resultados ---\n");
    printf("Valor final do contador: %lld\n", global_sum);
    printf("Valor esperado:          %lld\n", expected);
    
    if (global_sum != expected) {
        printf("Diferenca: %lld. CONDICAO DE CORRIDA DETECTADA!\n", expected - global_sum);
    } else {
        printf("Resultado correto!\n");
    }
    
    return 0;
}