#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "semafori.h"

#define MUTEX 0
#define WRITE   1

int main(int argc, char *argv[]) 
{
    key_t shm_key = atoi(argv[1]);
    key_t sem_key = atoi(argv[2]);
    int N = atoi(argv[3]);
    int id = atoi(argv[4]);

    // collegamento alle risorse
    int shm_id = shmget(shm_key, 0, 0664);
    char *shm_ptr = shmat(shm_id, NULL, 0);
    int *reader_count = (int *)shm_ptr;
    char *shared_str = (char *)(reader_count + 1);

    int sem_id = semget(sem_key, 2, 0664);

    for (int i = 0; i < 7; i++) {
        sleep(2);

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

        int len = strlen(buffer);
        for (int j = 0; j < len / 2; j++) 
        {
            char tmp = buffer[j];
            buffer[j] = buffer[len - j - 1];
            buffer[len - j - 1] = tmp;
        }

        printf("[Analizzatore %d] Invertita: %s\n", id, buffer);

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
