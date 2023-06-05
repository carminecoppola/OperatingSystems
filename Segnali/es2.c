/*  Un processo padre crea M (con M numero pari) processi figli. Ciascun processo figlio Fi è identificato
    da una variabile intera i (i=0,1,2,3, ..., M-1). Si gestiscano i seguenti due casi:
        1. Se il primo argomento dato in input al programma (ovvero argv[]) è uguale al carattere 'p'
           (che sta per "pari") ogni processo figlio Fi con i pari (ovvero se M=6, FO, F2, F4) manda 
           un segnale (SIGUSR1) al processo i + 1 (ovvero a F1, F3, F5 nel caso in cui M=6);

        2. Se invece arg[1] è uguale al carattere 'm' (che sta per "metà") ogni processo figlio Fi con
           I < M/2 (ovvero se M=6, FO, FI, F2) manda un segnale (SIGUSR1) all'altra metà del processi,
           ovvero al processo i + M/2 (ovvero a F3, F4, F5 nel caso in cui Me6); */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<unistd.h>

int M = 20; // Numero di processi figli da creare
char carattere; // Carattere da linea di comando

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER; // Mutex per garantire mutua esclusione


void signalHandler(int signum)
{
    printf("Processo %d ha ricevuto il segnale SIGUSR1\n", getpid());
}


/* In questa soluzione ho pensato di accedere alla variabile globale carattere
   in M.E. poichè condivisa dai thread, quindi per sicurezza la gestisco cosi,
   visto che al thread passo gia la 'i' ho reso 'carattere' globale. */

void *routine(void *arg)
{
   int i = *((int*)arg); // Indice del thread

   printf("Thread %d: Inizio routine\n", i);

   pthread_mutex_lock(&myMutex); // Acquisizione del lock per mutua esclusione
   printf("Thread %d: Acquisito il lock\n", i);

   if (carattere == 'p')
   {
      int pidFiglio = getpid() + i + 1;
      printf("Thread %d: Invio segnale a processo %d\n", i, pidFiglio);
      kill(pidFiglio, SIGUSR1); // Invio del segnale SIGUSR1 al processo figlio successivo
   }
   else if (carattere == 'm')
   {
      int pidFiglio = getpid() + ((i + M/2) % M);
      printf("Thread %d: Invio segnale a processo %d\n", i, pidFiglio);
      kill(pidFiglio, SIGUSR1); // Invio del segnale SIGUSR1 all'altro processo figlio
   }
   else
   {
      printf("Thread %d: Carattere inserito non valido\n", i);
   }

   printf("Thread %d: Rilasciato il lock\n", i);
   pthread_mutex_unlock(&myMutex); // Rilascio del lock

   printf("Thread %d: Fine routine\n", i);
   
   pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
   if (argc != 2)
   {
      printf("Utilizzo: %s <carattere>\n", argv[0]);
      return -1;
   }

   carattere = argv[1][0]; // Impostazione del carattere preso dalla linea di comando

   signal(SIGUSR1, signalHandler); // Registra la funzione di gestione dei segnali

   pthread_t th[M]; // Array di thread
   int indices[M]; // Array di indici per i thread

   for (int i = 0; i < M; i++)
   {
      indices[i] = i;

      if (pthread_create(&th[i], NULL, routine, (void*)&indices[i]) != 0)
      {
         printf("\nErrore nella creazione dei thread\n");
         return -1;
      }
   }

   for (int i = 0; i < M; i++)
   {
      if (pthread_join(th[i], NULL) != 0)
      {
         printf("\nErrore nella fase di JOIN dei thread\n");
         return -2;
      }
   }

   return 0;
}
