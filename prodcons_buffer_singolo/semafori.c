#include <stdio.h>

#include "semafori.h"

/*
acquisisce la risorsa (decrementa il valore del semaforo)
- S > 0 continua l'esecuzione (risorsa subito disponibile)
- S <= 0 la risorsa è occupata e o ci sono altri processi in attesa (il processo si bloccca)
*/
int Wait_Sem (int id_sem, int numsem) 
{
       int err; // salva il valore di ritorno di semop
       struct sembuf sem_buf;      

       sem_buf.sem_num = numsem; // per identificare il semaforo all'interno dell'array
       sem_buf.sem_flg = 0; 
       sem_buf.sem_op = -1; // valore da assegnare al semaforo

       err = semop(id_sem,&sem_buf,1);   //semaforo rosso

       if(err < 0) // controlla se ci sono erroru
       {
              perror("Errore WAIT");
       }

       return err;
}

/* 
rilascia la risorsa (incrementa il valore semaforo)
- S <= 0 libera un processo in attesa 
- S > 0 nessun processo in attesa
*/
int Signal_Sem (int id_sem, int numsem) 
{
       int err;
       struct sembuf sem_buf;

       sem_buf.sem_num = numsem;
       sem_buf.sem_flg = 0;
       sem_buf.sem_op = 1;

       err = semop(id_sem,&sem_buf,1);   //semaforo verde

       if(err<0) 
       {
         perror("Errore SIGNAL");
       }

       return err;
}