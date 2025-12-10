#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
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
    int *reader_count = (int *)shm_ptr;
    char *shared_str = (char *)(reader_count + 1);

    int sem_id = semget(sem_key, 2, 0644);

    for (int i = 0; i < 10; i++) 
    {
        sleep(1);

        // entra nella lettura
        wait_sem(sem_id, MUTEX);
        (*reader_count)++;
        if (*reader_count == 1)
        {
            wait_sem(sem_id, WRITE);
        }   
        signal_sem(sem_id, MUTEX);

        // legge
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "%s", shared_str);
        for (int j = 0; buffer[j]; j++)
        {
            buffer[j] = toupper(buffer[j]);
        }
        printf("[Elaboratore] Maiuscolo: %s\n", buffer);

        // esce dalla lettura
        wait_sem(sem_id, MUTEX);
        (*reader_count)--;
        if (*reader_count == 0)
        {
            signal_sem(sem_id, WRITE);
        }
        signal_sem(sem_id, MUTEX);
    }

    shmdt(shm_ptr);
    return 0;
}
