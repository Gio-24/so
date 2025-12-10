#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "semafori.h"

#define MUTEX 0
#define WRITE 1

int main(int argc, char *argv[])
{
    key_t shm_key = atoi(argv[1]);
    key_t sem_key = atoi(argv[2]);
    int N = atoi(argv[3]);

    // collegamento alla memoria condivisa
    int shm_id = shmget(shm_key, 0, 0644);
    char *shm_ptr = shmat(shm_id, NULL, 0);
    int *reader_count = (int *) shm_ptr;
    char *shared_str = (char *)(reader_count + 1);

    // collegamento ai semafori
    int sem_id = semget(sem_key, 2, 0644);

    srand(time(NULL));

    for (int i = 0; i < 5; i++) 
    {
        sleep(3);

        // sezione critica
        wait_sem(sem_id, WRITE);   // semaforo rosso (blocca i lettori)

        sleep(1);

        for (int j = 0; j < N; j++)
        {
            shared_str[j] = 'a' + (rand() % 26);
        }
        
        shared_str[N] = '\0';

        printf("[Generatore] Nuova stringa: %s\n", shared_str);

        signal_sem(sem_id, WRITE);
    }

    shmdt(shm_ptr);
    return 0;
}
