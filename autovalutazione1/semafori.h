#ifndef _SEMAFORI_H_
#define _SEMAFORI_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int wait_sem(int id_sem, int numsem);
int signal_sem(int id_sem, int numsem);

#endif // _SEMAFORI_H_