#include <pthread.h> // libreria dinamica ==> parametro -pthread durante l'invocazione del compilatore
#include <stdio.h>
#include <stdlib.h> // per le funzioni malloc e free

#define NUM_THREADS 5 // macro che definisce il numero massimo di thread


/* 
la start routine o RAM function è la procedura che viene associata ad un thread, all'atto 
della creazione ed è quello che il thread eseguirà

N.B. è di tipo void* è contiene un parametro, che può essere usato per I/O, di tipo void*
*/
void * PrintHello(void * arg) // start routine
{
    int *p_arg = (int*) arg; // casting del parametro passato al thread da void* a int*

    printf("\n%d: Hello World! (arg address: %p)\n", *p_arg, p_arg);

    free(p_arg); // libera la memoria

    pthread_exit(NULL); // terminazione thread "figlio"
}

int main (int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS]; // array di thread di tipo pthread_t

    int rc; // reuturn code

    for(int i = 0; i < NUM_THREADS; i++)
    {
        printf("Creating thread %d\n", i);

        /*
        N.B. c'è bisogno di fare una malloc perchè tutti i thread hanno stack diversi ma 
        condividono la heap. 
        */
        int *ptr_i = malloc(sizeof(int)); // varibile condivisa tra i thread

        *ptr_i = i;

        /*
        la funzione pthread_create crea un thread ed esegue subito la sua ram function

        parametri
        - indirizzo di un varibile thread
        - riferimento ad una varibile attributo --> definisce alcuni comportamenti specifici come essere joinable
        - start routine
        - parametro 
        */
        rc = pthread_create(&threads[i], NULL, PrintHello, (void *) ptr_i);

        if (rc != 0)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // si usa al posto di del return 0 per permettere agli altri thread di continuare la loro esecuzione
    pthread_exit(NULL); // terminazione del thread
}