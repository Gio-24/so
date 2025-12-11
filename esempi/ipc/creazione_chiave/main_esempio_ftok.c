/*esempio IPC*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>

/*
ogni oggetto IPC è identificato da una chiave. La chiave può essere create in 3 modi
- hardcoded: ovvero scelta arbitrariamente
  N.B. se la chiave scelta esiste già può causare problemi/conflitti
- ftok(): ovvero scelta dal sistema tra quelle disponibili
- IPC_PRIVATE: ovvero assume valore zero ed è visibile soltanto al processo creatore
  e ai suoi figli.
*/

int main()
{
  key_t my_key;

  my_key = ftok("./eseguibile", 'a'); 
  /* 
  N.B. il primo parametro serve per indicare l'applicazione di appartenenza (di solito un file comune)
  mentre il secondo per distinguere molteplici risorse di una stessa applicazione 
  */

  return 0;
}