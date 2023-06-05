/*  Scrivere un programma in C che in ambiente Linux e utilizzando o libreria pthread crei 2 thread che eseguono
    la funzione "incrementa" che a sua volta accede alle variabili glob.a e glob.b di una struttura dati 
    condivisa glob e ne incrementi il loro valore di 1 per 100 volte. Al termine quando 2 thread avranno 
    terminato con gli incrementi, il thread principale stampera a vido i valori delle variabili test.a
    e test.b . Per la gestione della sincronizzazione si utilizzano mutex allocati dinamcamente.  */ 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct
{
    int glob_a;
    int glob_b;
} glob;

// Funzione eseguita dai thread per incrementare le variabili condivise
void *incrementa(void *arg)
{
    glob *testGlob = (glob *)arg;

    for (int i = 0; i < 100; i++)
    {
        pthread_mutex_lock(&myMutex);  // Acquisizione del mutex per garantire l'accesso esclusivo alle variabili condivise
            testGlob->glob_a++; // Incremento di glob_a
            testGlob->glob_b++; // Incremento di glob_b
        pthread_mutex_unlock(&myMutex);   // Rilascio del mutex
    }

    pthread_exit(NULL);
}

// Funzione eseguita dal thread principale per stampare i valori delle variabili condivise
void *stampa(void *arg)
{
    glob *testGlob = (glob *)arg;

    int a = testGlob->glob_a;
    int b = testGlob->glob_b;

    pthread_mutex_lock(&myMutex);   // Acquisizione del mutex per garantire l'accesso esclusivo alle variabili condivise
        printf("\nEcco glob.a: %d", a); // Stampa il valore di glob_a
        printf("\nEcco glob.b: %d\n", b); // Stampa il valore di glob_b
    pthread_mutex_unlock(&myMutex);  // Rilascio del mutex

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    glob *dataGlob = (glob *)malloc(sizeof(glob));

    dataGlob->glob_a = 0;
    dataGlob->glob_b = 0;

    pthread_t th1, th2, finalThread;

    // Creazione dei thread che eseguono la funzione incrementa
    if (pthread_create(&th1, NULL, incrementa, (void *)dataGlob) != 0)
    {
        printf("\nErrore nella creazione dei thread\n");
        return -1;
    }

    if (pthread_create(&th2, NULL, incrementa, (void *)dataGlob) != 0)
    {
        printf("\nErrore nella creazione dei thread\n");
        return -1;
    }

    // Fase di JOIN per attendere la terminazione dei thread incrementa
    if (pthread_join(th1, NULL) != 0)
    {
        printf("\nErrore nella fase di JOIN dei thread\n");
        return -2;
    }

    if (pthread_join(th2, NULL) != 0)
    {
        printf("\nErrore nella fase di JOIN dei thread\n");
        return -2;
    }

    // Creazione del thread finale per stampare i valori
    if (pthread_create(&finalThread, NULL, stampa, (void *)dataGlob) != 0)
    {
        printf("\nErrore nella creazione dei thread\n");
        return -1;
    }

    if (pthread_join(finalThread, NULL) != 0)
    {
        printf("\nErrore nella fase di JOIN dei thread\n");
        return -2;
    }

    return 0;
}
