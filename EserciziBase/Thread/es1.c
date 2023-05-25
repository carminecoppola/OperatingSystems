#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Struttura per i parametri del thread
struct parametri
{
    int num_thread;
    char messaggio;
};

// Funzione eseguita dal thread
void *printHello(void *arg)
{
    // Casting del puntatore all'argomento
    struct parametri *params = (struct parametri *)arg;

    // Stampa del numero del thread e del messaggio
    printf("Thread %d: %c\n", params->num_thread, params->messaggio);

    // Terminazione del thread
    pthread_exit(NULL);
}

int main()
{
    // Dichiarazione dell'array di thread
    pthread_t threads[5];

    // Dichiarazione dell'array di parametri
    struct parametri params[5];

    // Ciclo per creare i thread
    for (int i = 0; i < 5; i++)
    {
        printf("Creazione thread %d\n", i);

        // Assegnazione dei valori ai parametri del thread corrente
        params[i].num_thread = i;
        params[i].messaggio = 'A' + i;

        // Creazione del thread passando la funzione, l'argomento e l'indirizzo del thread corrente
        int th = pthread_create(&threads[i], NULL, printHello, (void *)&params[i]);
    
        // Controllo degli errori nella creazione del thread
        if (th)
        {
            printf("ERROR, return code from pthread_create() is %d\n", th); 
            exit(-1);
        }
    }

    // Attendiamo la terminazione dei thread
    for (int i = 0; i < 5; i++)
    {
        // Utilizziamo la funzione pthread_join per attendere la terminazione di ciascun thread
        pthread_join(threads[i], NULL);
    }

    return 0;
}
