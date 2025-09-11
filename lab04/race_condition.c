#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 8
#define INCREMENTS_PER_THREAD 1000000

long long counter = 0;

void* thread_func(void* arg) {
    long thread_id = (long)arg;
    printf("Thread %ld iniciando o trabalho...\n", thread_id);
    
    for (int i = 0; i < INCREMENTS_PER_THREAD; ++i) {
        counter++;
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    for (long i = 0; i < NUM_THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, thread_func, (void*)i) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    long long expected = (long long)NUM_THREADS * INCREMENTS_PER_THREAD;
    printf("\n--- Resultados ---\n");
    printf("Valor final do contador: %lld\n", counter);
    printf("Valor esperado:          %lld\n", expected);
    
    if (counter != expected) {
        printf("Diferenca: %lld. CONDICAO DE CORRIDA DETECTADA!\n", expected - counter);
    } else {
        printf("Resultado correto!\n");
    }
    
    return 0;
}