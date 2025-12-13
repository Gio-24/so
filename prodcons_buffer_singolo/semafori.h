#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#include <sys/types.h>        
#include <sys/ipc.h>
#include <sys/sem.h>

int Wait_Sem(int id_sem, int numsem); // funzione per acquisire la risorsa
int Signal_Sem(int id_sem, int numsem); // funzione per rilasciare la risorsa

#endif // _PROCEDURE_H_