#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main () {
    int pipefd [2];
    char buffer [100];
    
    if (pipe (pipefd) == -1) {
        perror ("pipe");
        exit (EXIT_FAILURE);
    }
    
    pid_t pid = fork ();
    
    if (pid < 0) {
        perror ("fork");
        exit (EXIT_FAILURE);
    }

    if (pid == 0) {
        close (pipefd [0]);
        const char * msg = "Olá do filho!";
        printf ("Filho: enviando mensagem ...\n");
        write (pipefd [1], msg, strlen ( msg ) + 1);
        close (pipefd [1]);
        exit (EXIT_SUCCESS);
    } 
    
    else {
        close (pipefd [1]);
        printf ("Pai: esperando mensagem ...\n");
        read (pipefd [0], buffer, sizeof (buffer));
        printf ("Pai: recebeu a mensagem: ’%s’\n", buffer );
        close (pipefd [0]);
        wait (NULL);
    }

    return 0;
}