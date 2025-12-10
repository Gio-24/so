#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <string.h>


#define MUTEX 0
#define WRITE 1

int main()
{
    // generazione delle chiavi 
    key_t shm_key = ftok("main.c", 'a');
    key_t sem_key = ftok("main.c", 'b');

    // lunghezza stringa
    srand(time(NULL));
    int N = 10+ rand() % 6; // (10 - 15)

    // creazione memoria condivisa
    int size = sizeof(int) + (N + 1) * sizeof(char); // contatore + stringa di N + 1 caratteri
    int shm_id = shmget(shm_key, size, IPC_CREAT | 0644);
    if(shm_id == -1) // controlla se la memoria è stata allocata con successo
    {
        perror("Errore creazione memoria condivisa");
        exit(1);
    }

    // collegamento memoria condivisa
    void *shm_ptr = shmat(shm_id, NULL, 0);
    if(shm_ptr == (void*)-1) // controlla se il collegamento è avventuo con successo
    {
        perror("Errore collegamento memoria condivisa");
        exit(1);
    }

    // dichiarazione ed inizializzazione contatore
    int *reader_count = (int *) shm_ptr;
    *reader_count = 0;

    // dichiarazione ed inizializzazione stringa
    char *shared_str = (char *)(reader_count + 1);
    shared_str[0] = '\0'; // stringa vuota

    // creazione semafori
    int sem_id = semget(sem_key, 2, IPC_CREAT | 0644);
    if(sem_id == -1)
    {
        perror("Errore creazione semafori");
        exit(1);
    }

    // inizializzazione semafori
     if(semctl(sem_id, MUTEX, SETVAL, 1) == -1) {
        perror("Errore semafori");
        exit(1);
    }

    if(semctl(sem_id, WRITE, SETVAL, 1) == -1) {
        perror("Errore semafori");
        exit(1);
    }

    printf("[MAIN] N=%d, shm_key=%d, sem_key=%d\n", N, shm_key, sem_key);

    // exec 
    char shm_key_str[20], sem_key_str[20], n_str[10];
    sprintf(shm_key_str, "%d", shm_key);
    sprintf(sem_key_str, "%d", sem_key);
    sprintf(n_str, "%d", N);

    // processi figli
    pid_t pid;

    if((pid = fork()) == 0) 
    {
        execl("./generatore", "generatore", shm_key_str, sem_key_str, n_str, NULL);
        perror("Exec generatore fallita");
        exit(1);
    }

    if((pid = fork()) == 0) 
    {
        execl("./elaboratore", "elaboratore", shm_key_str, sem_key_str, n_str, NULL);
        perror("Exec elaboratore fallita");
        exit(1);
    }

    if((pid = fork()) == 0) 
    {
        execl("./analizzatore", "analizzatore", shm_key_str, sem_key_str, n_str, "1", NULL);
        perror("Exec analizzatore1 fallita");
        exit(1);
    }

    if((pid = fork()) == 0) 
    {
        execl("./analizzatore", "analizzatore", shm_key_str, sem_key_str, n_str, "2", NULL);
        perror("Exec analizzatore2 fallita");
        exit(1);
    }

    // rimozione risorse
    shmdt(shm_ptr);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);

    printf("[MAIN] Tutte le risorse liberate\n");
    return 0;
}
