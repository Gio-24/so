#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

#include "procedure.h"
#include "semafori.h"


void produttore(int * p, int ds_sem) // si passa il riferimento al segmento di memoria condivisa e il descrittore del semaforo
{
	printf("produttore è fermo prima di wait\n");
	Wait_Sem(ds_sem, SPAZIO_DISPONIBILE); // decrementa il valore del semaforo del produttore e si mette in attesa
	printf("produttore si sblocca dopo la wait\n");


	sleep(2);

  srand(time(NULL));

	*p = rand() % 100;

	printf("Il valore prodotto = %d\n", *p);


	Signal_Sem(ds_sem, MESSAGGIO_DISPONIBILE); // incrementa il semaforo della consumazione (segnala che ha prodotto)
}

void consumatore(int * p, int ds_sem) 
{

	printf("consumatore è fermo prima di wait\n");
	Wait_Sem(ds_sem, MESSAGGIO_DISPONIBILE); // si mette in attesa per la consumazione
	printf("consumatore si sblocca dopo la wait\n");


	sleep(2);
	printf("Il valore consumato = %d\n", *p);


	Signal_Sem(ds_sem, SPAZIO_DISPONIBILE);
}