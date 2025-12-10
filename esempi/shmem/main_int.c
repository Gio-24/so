/* esempio workflow shared memory*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>  // per la creazione di una chiave IPC
#include <sys/shm.h>    // funzioni per la gestione di una SHM
#include <sys/ipc.h>    // funzioni per la gestione di una IPC
#include <sys/wait.h>

#define SHM_SIZE sizeof(int)    // dimensione della shared memory (in byte)

/* 
workflow shared memory
- creazione ==> shmget
- collegamento ==> shmat
- utilizzo
- scollegamento ==> shmdt (rilascio locale della risorsa)
- eliminazione ==> shmctl (rilascio globale della risorsa)
*/

int main()
{
    // creazione di una chiave per la SHM
    key_t shm_key = IPC_PRIVATE; // utilizzabile solo dal creatore e dai figli

    /* creazione della memoria */
    int ds_shm = shmget(shm_key, SHM_SIZE, IPC_CREAT | 0664);
    if(ds_shm < 0) // controlla l'esito della shmget
    { 
 		perror("errore shmget!");
 		exit(1); 
 	}

    /* collegamento della memoria */
    int * ptr_shm = (int *) shmat(ds_shm, NULL, 0);

    int pid = fork(); // esegue la fork e restituisce il pid del processo figlio

    if(pid == 0)
    {
        //processo figlio
        *ptr_shm = 1;
        exit(0);
 	} 
    else if (pid > 0) 
    {
        // processo padre   
        wait(NULL);
        printf("Contenuto SHM: %d\n", *ptr_shm);
 	}

    /* eliminazione della memoria */
    shmctl(ds_shm, IPC_RMID, NULL);

    return 0;
}