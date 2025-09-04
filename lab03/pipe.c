#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define VECTOR_SIZE 1000
#define NUM_PROCESSES 4

long long sum_chunk(int* vector, int start, int end) {
    long long sum = 0;
    for (int i = start; i < end; i++) {
        sum += vector[i];
    }
    return sum;
}

int main() {
    int vector[VECTOR_SIZE];
    long long total_sum_expected = 0;

    for (int i = 0; i < VECTOR_SIZE; i++) {
        vector[i] = i + 1;
        total_sum_expected += vector[i];
    }

    int pipes[NUM_PROCESSES][2];
    pid_t pid;
    int chunk_size = VECTOR_SIZE / NUM_PROCESSES;

    for (int i = 0; i < NUM_PROCESSES; i++) {

        if (pipe(pipes[i]) == -1) {
            fprintf(stderr, "Falha ao criar o pipe para o filho %d!\n", i);
            return 1;
        }

        pid = fork();

        if (pid < 0) {
            fprintf(stderr, "Fork falhou!\n");
            return 1;
        }

        if (pid == 0) {

            close(pipes[i][0]);
            
            for (int j = 0; j < NUM_PROCESSES; j++) {
                if (i != j) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
            }

            int start = i * chunk_size;
            int end = (i == NUM_PROCESSES - 1) ? VECTOR_SIZE : (i + 1) * chunk_size;

            long long partial_sum = sum_chunk(vector, start, end);
            printf("[FILHO %d] Soma parcial calculada: %lld. Enviando...\n", getpid(), partial_sum);

            write(pipes[i][1], &partial_sum, sizeof(long long));

            close(pipes[i][1]);
            exit(0);
        }
    }

    long long final_sum = 0;

    for (int i = 0; i < NUM_PROCESSES; i++) {

        close(pipes[i][1]);

        long long partial_sum_from_child;

        read(pipes[i][0], &partial_sum_from_child, sizeof(long long));
        
        printf("[PAI] Recebido %lld do filho do pipe %d.\n", partial_sum_from_child, i);
        final_sum += partial_sum_from_child;

        close(pipes[i][0]);
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }
    
    printf("\n----------------------------------------\n");
    printf("Soma final calculada pelo pai: %lld\n", final_sum);
    printf("Soma esperada: %lld\n", total_sum_expected);
    printf("----------------------------------------\n");

    return 0;
}