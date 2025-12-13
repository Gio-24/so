#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

#include "procedure.h"

int main() 
{
	srand(time(NULL));

	key_t chiave = IPC_PRIVATE; // crea una chiave privata

	int ds_shm = shmget(chiave, sizeof(int), IPC_CREAT|0664); // pttiene il descrittore della risorsa

	if(ds_shm<0) { perror("SHM errore"); exit(1); }

	int * p;

	p = (int *) shmat(ds_shm, NULL, 0); // ottiene un riferimento alla risorsa



	key_t chiavesem = IPC_PRIVATE;

	int ds_sem = semget(chiavesem, 2, IPC_CREAT|0664);

	if(ds_sem<0) { perror("SEM errore"); exit(1); }

	
	*p=0;

  //SEMAFORI COOPERAZIONE tra 1 prod e 1 cons: NO COMPETIZIONE
	semctl(ds_sem, SPAZIO_DISPONIBILE, SETVAL, 1); // inzializza il semaforo della produzione ad 1 (buffer vuoto ed è possibile produrre)
	semctl(ds_sem, MESSAGGIO_DISPONIBILE, SETVAL, 0); // inizializza il semaforo per la consumazione a 0 (buffer vuoto non è possibile consumare)



	int pid = fork();

	if(pid == 0) {
		//figlio consumatore
		printf("Inizio figlio consumatore\n");
		consumatore(p, ds_sem);
		exit(1);
	}


	pid = fork();

	if(pid==0) {
		//figlio produttore
		printf("Inizio figlio produttore\n");
		produttore(p, ds_sem);
		exit(1);
	}



	wait(NULL);
	printf("primo figlio terminato\n");

	wait(NULL);
	printf("secondo figlio terminato\n");

        shmctl(ds_shm, IPC_RMID, NULL); // rimuove la memoria condivisa
       	semctl(ds_sem, 0, IPC_RMID); // rimuove il semaforo per la produzione
				semctl(ds_sem, 1, IPC_RMID); // rimuove il semaforo per la consumazione
        
        return 0;
}