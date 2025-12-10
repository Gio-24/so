#include <stdio.h>

#include "semafori.h"

int wait_sem(int id_sem, int numsem)
{
    int err;
    struct sembuf sem_buf;

    sem_buf.sem_num = numsem;
    sem_buf.sem_flg = 0;
    sem_buf.sem_op = -1;

    err = semop(id_sem, &sem_buf, 1); // semaforo rosso

    if(err < 0)
    {
        perror("Errpre WAIT");
    }

    return err;
}

int signal_sem(int id_sem, int numsem)
{
    int err;
    struct sembuf sem_buf;

    sem_buf.sem_num = numsem;
    sem_buf.sem_flg = 0;
    sem_buf.sem_op = 1;

    err = semop(id_sem, &sem_buf, 1); // semaforo verde

    if(err < 0)
    {
        perror("Errore SIGNAL");
    }

    return err;
}