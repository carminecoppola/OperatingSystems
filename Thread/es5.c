/*  Scrivi un programma in C che utilizzi i thread per calcolare il fattoriale di un numero intero
    inserito dall'utente. Il programma dovrebbe creare un thread separato per eseguire il calcolo 
    del fattoriale e il thread principale dovrebbe stampare il risultato. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

unsigned long long fattoriale = 1; // Variabile globale per memorizzare il risultato del fattoriale

void *routine(void* args)
{
    int n = *((int*)args); // Ottiene il valore del numero intero da calcolare

    // Calcola il fattoriale
    for (int i = 1; i <= n; i++)
    {
        fattoriale *= i;
    }
    
    return NULL;
}

void *routineStampa(void* args)
{
    printf("\nIl fattoriale è uguale a %llu\n", fattoriale); // Stampa il risultato del fattoriale

    return NULL;
}

int main()
{
    pthread_t th1;
    pthread_t thFattoriale;

    int num = 0; 

    printf("\nInserisci il numero di cui vuoi calcolare il fattoriale:");
    scanf("%d", &num); // Legge il numero intero da calcolare

    if (pthread_create(&thFattoriale, NULL, &routine, (void*)&num) != 0)
    {
        printf("\nErrore durante la creazione del thread fattoriale");
        return -1;
    }

    if (pthread_join(thFattoriale, NULL) != 0)
    {
        printf("\nErrore durante la fase di JOIN del fattoriale");
        return -2;
    }

    // Secondo thread che effettua la stampa
    if (pthread_create(&th1, NULL, &routineStampa, NULL) != 0)
    {
        printf("\nErrore durante la creazione del thread che stampa");
        return -1;
    }

    if (pthread_join(th1, NULL) != 0)
    {
        printf("\nErrore durante la fase di JOIN");
        return -2;
    }
    
    return 0;
}
