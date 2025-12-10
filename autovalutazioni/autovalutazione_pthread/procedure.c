#include "header.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <time.h>

void inizializza(struct monitor* m){

	m->molo=0;
	m->id_nave=0;

    /* TODO: Inizializzare le variabili dell'algoritmo, il mutex, e le variabili condition */

    /* inizializzazione mutex */
    pthread_mutex_init(&m->mutex, NULL); // stato unlocked

    /* inizializzazione cv */
    pthread_cond_init(&m->lettura_cv, NULL); // inizializzazione lettori
    pthread_cond_init(&m->scrittura_cv, NULL); // inizializzazione scrittori

    /* inizializzazione variabili d'utilità */
    m->num_lettori = 0; // inizialmente nessun lettore attivo
    m->num_lettori_wait = 0; // inizialmente 10 possibili thread lettori (viaggiatori)

    m->num_scrittori = 0; // inizialmente nessuno scrittore attivo
    m->num_scrittori_wait = 0; // inizialmente 5 possibili thread scrittori (gestori molo)

}

void rimuovi (struct monitor* m){
    /* TODO: Disattivare mutex e variabili condition */
    pthread_mutex_destroy(&m->mutex); // disattiva il mutex
    pthread_cond_destroy(&m->lettura_cv); //disattiva cv lettori
    pthread_cond_destroy(&m->scrittura_cv); // disattiva cv scrittori
}


//SCRITTURA. AGGIORNAMENTO DELLA POSIZIONE DEL TRENO
void scrivi_molo(struct monitor* m, int molo){

    /* TODO: Implementare qui lo schema dei lettori-scrittori con starvation di entrambi.
     * nella parte della SCRITTURA
     */

    /* inizio scrittura */
    pthread_mutex_lock(&m->mutex); // entra nel monitor
    
    while (m->num_lettori > 0 || m->num_scrittori > 0) // attende finchè ci sono lettori o scrittori attivi
    {
        m->num_scrittori_wait++; // incrementa il num di scrittori in attesa
        pthread_cond_wait(&m->scrittura_cv, &m->mutex); // si mette in attesa
        m->num_scrittori_wait--; // diminuisce il num di scrittori in attesa
    }

    m->num_scrittori++; // incrementa il num di scrittori attivi

    pthread_mutex_unlock(&m->mutex); // esce dal monitor

    /* sezione critica */
    m->molo = molo;

    /* fine scrittura */
    pthread_mutex_lock(&m->mutex);

    m->num_scrittori--;

    if(m->num_scrittori_wait > 0) // controlla se ci sono scrittori in attesa
    {
        pthread_cond_signal(&m->scrittura_cv); // precedenza agli scrittori
    }
    else // altrimenti 
    {
        pthread_cond_broadcast(&m->lettura_cv); // risveglia tutti i lettori
    }

    pthread_mutex_unlock(&m->mutex); // esci dal monitor
}


//LETTURA. RESTITUISCE LA POSIZIONE DEL TRENO
int leggi_molo(struct monitor* m){

    /* TODO: Implementare qui lo schema dei lettori-scrittori con starvation di entrambi.
     * nella parte della LETTURA
     */

    int val = 0; // valore da leggere
    
    /* inizio lettura */
    pthread_mutex_lock(&m->mutex); // entra nel monitor

    while(m->num_scrittori > 0) // attende finchè ci sono scrittori attivi
    {
        m->num_lettori_wait++; // incrementa il num di lettori in attesa
        pthread_cond_wait(&m->lettura_cv, &m->mutex); // si mette in attesa
        m->num_lettori_wait--; // decrementa il numero di lettori in attesa
    }

    m->num_lettori++; // incrmenta il num di lettori attivi

    pthread_mutex_unlock(&m->mutex); // esce dal monitor

    /* sezione critica */
    val = m->molo;

    /* fine lettura */
    pthread_mutex_lock(&m->mutex);

    m->num_lettori--; // decrementa il numero di lettori attivi

    if(m->num_lettori == 0) // se non ci sono lettori attivi
    {
        pthread_cond_signal(&m->scrittura_cv); // risveglia lettori in attesa
    }

    pthread_mutex_unlock(&m->mutex);

    return val;
}

