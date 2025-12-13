/* definisce i prototipo per le funzioni di produzione e consumazione */

#define SPAZIO_DISPONIBILE 0 // semaforo per la produzuione
#define MESSAGGIO_DISPONIBILE 1 // semaforo per la consumazione

void produttore(int *, int);
void consumatore(int *, int);