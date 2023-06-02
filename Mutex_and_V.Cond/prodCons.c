/*  Scrivere un programma C e Posix sotto Linux che, utilizzando la libreria Pthread crei due Thread, 
    chiamati produttore e consumatore. 
    La risorsa condivisa, è un buffer circolare di dimensione data (ad esempio 20) il cui stato è :
        • numero di elemento contenuti: count;
        • puntatore alla prima posizione libera: writepos;
        • puntatore al primo elemento occupato: readpos;
    II produttore, inserisce, 20 numeri interi in maniera sequenziale. 
    Il consumatore li estrae sequenzialmente per stamparli. 
    Il programma dovrà prevedere:
        • un meccanismo di accesso controllato alla risorsa buffer da parte dei due Thread 
          (mutex per il controllo della mutua esclusione nell' accesso al buffer)
        • una sincronizzazione tra il produttore ed il consumatore (Thread) in caso di:
            • buffer pieno: definizione di una condition per la sospensione del produttore
              se il buffer è pieno (notfull);
            • buffer vuoto: definizione di una condition per la sospensione del consumatore 
              se il buffer è vuoto (notempty);
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define BUFFER_SIZE 20

int buffer[BUFFER_SIZE];        // Buffer circolare di dimensione 20
int count = 0;                  // Numero di elementi contenuti nel buffer
int writepos = 0;               // Posizione della prima posizione libera
int readpos = 0;                // Posizione del primo elemento occupato

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex per il controllo dell'accesso esclusivo al buffer
pthread_cond_t notfull = PTHREAD_COND_INITIALIZER;     // Condition variable per il controllo del buffer pieno
pthread_cond_t notempty = PTHREAD_COND_INITIALIZER;    // Condition variable per il controllo del buffer vuoto

void printBuffer()
{
    printf("Buffer attuale: ");
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        printf("%d ", buffer[i]);
    }
    printf("\n");
}

void* routineProduttore()
{
    for (int i = 0; i < 20; i++)
    {
        pthread_mutex_lock(&myMutex);   // Acquisizione del lock sul mutex
        
        while (count == BUFFER_SIZE)
        {
            // Se il buffer è pieno, il produttore si sospende e attende
            // che il consumatore consumi alcuni elementi per liberare spazio
            pthread_cond_wait(&notfull, &myMutex);
        }

        int num = rand() % 99 + 1;
        buffer[writepos] = num;
        writepos = (writepos + 1) % BUFFER_SIZE;
        count++;
        
        printf("Produttore: Generato il numero %d\n", num);
        printBuffer();
        
        pthread_cond_signal(&notempty);  // Segnala al consumatore che il buffer non è più vuoto
        pthread_mutex_unlock(&myMutex); // Rilascio del lock sul mutex
    }

    pthread_exit(NULL);
}

void* routineConsumatore()
{
    for (int i = 0; i < 20; i++)
    {
        pthread_mutex_lock(&myMutex);   // Acquisizione del lock sul mutex
        
        while (count == 0)
        {
            // Se il buffer è vuoto, il consumatore si sospende e attende
            // che il produttore produca alcuni elementi da consumare
            pthread_cond_wait(&notempty, &myMutex);
        }
        
        int value = buffer[readpos];
        buffer[readpos] = 0; // Rimuovi l'elemento dal buffer
        readpos = (readpos + 1) % BUFFER_SIZE;
        count--;
        
        printf("Consumatore: Elemento estratto %d\n", value);
        printBuffer();
        
        pthread_cond_signal(&notfull);   // Segnala al produttore che il buffer non è più pieno
        pthread_mutex_unlock(&myMutex); // Rilascio del lock sul mutex
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t thProduttore, thConsumatore;

    srand(time(NULL));

    // Creazione dei thread
    if (pthread_create(&thProduttore, NULL, routineProduttore, NULL) != 0)
    {
        printf("\nErrore nella creazione del thread produttore");
        return -1;
    }
    
    if (pthread_create(&thConsumatore, NULL, routineConsumatore, NULL) != 0)
    {
        printf("\nErrore nella creazione del thread consumatore");
        return -1;
    }

    // Fase di JOIN
    if (pthread_join(thProduttore, NULL) != 0)
    {
        printf("\nErrore nella fase di JOIN del thread produttore");
        return -2;
    }
    
    if (pthread_join(thConsumatore, NULL) != 0)
    {
        printf("\nErrore nella fase di JOIN del thread consumatore");
        return -2;
    }

    return 0;
}