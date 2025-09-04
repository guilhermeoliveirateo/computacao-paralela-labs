# include <stdio.h>
 # include <unistd.h>
 # include <sys/wait.h>

 int main () {

    pid_t pid;
    printf("Digite o valor de n: ");
    int n = 0;
    scanf("%d", &n);

    for (int i = 0; i < n; i++){
         pid = fork ();
         if ( pid < 0) { 
            printf ("Fork falhou!\n");
            return 1;
         } 
         else if (pid == 0) {
            printf ("Eu sou o filho! Meu PID e %d, meu pai e %d.\n", getpid () , getppid ());
            return 0;
         }
    }

    for (int i = 0; i < n; i++){
        wait(NULL);
    }

    printf ("Meus filhos terminaram, agora eu posso terminar.\n");

    return 0;
}