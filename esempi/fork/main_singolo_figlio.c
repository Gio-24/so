#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid; // signed integer

    pid = fork(); // system call che permette creare una copia di un processo

    /*
    N.B. la copia fatta dalla fork comprende le aree dati globali, stack, heap e U-area
    - il figlio eredita gli stessi valori delle variabili, i descrittori agli stessi file aperti, e anche il Program Counter (PC).
    - le modifiche apportate alle proprie variabili da uno dei due processi non sono visibili all’altro.

    Condividono la Text Area ==> eseguono lo stesso codice.
    */

    if(pid == -1) // errore nella fork
    {
        fprintf(stderr, "fork failed!\n");
        exit(1); // status != 0 --> errore/fallimento
    }
    else if(pid > 0) 
    {
        pid_t mypid = getpid();
        printf("Sono il padre, il mio PID è %d\n", mypid);
    } 
    else if(pid == 0)
    {
        pid_t mypid = getpid();
        printf("Sono il figlio, il mio PID è %d\n", mypid);
    }

    return 0;
}