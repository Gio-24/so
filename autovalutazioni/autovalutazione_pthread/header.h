#ifndef HEADER_H
#define HEADER_H

#include <bits/pthreadtypes.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>



struct monitor {

	int molo;
	int id_nave;

	/* TODO: Aggiungere variabili per la sincronizzazione */

	pthread_mutex_t mutex; // mutex monitor

	/* variable condition */
	pthread_cond_t lettura_cv; // ok lettura
	pthread_cond_t scrittura_cv; // ok scrittura

	/* varibili d'utilità */
	int num_lettori; // lettori attivi
	int num_lettori_wait; // lettori in attesa
	int num_scrittori; // scrittori attibi
	int num_scrittori_wait; //scrittori in attesa
};

void inizializza(struct monitor * m);
void rimuovi (struct monitor * m);
void scrivi_molo(struct monitor * m, int molo);
int leggi_molo(struct monitor * m);



#endif