/* Scrivi un programma in C che utilizzi i thread per calcolare la somma degli elementi
   in un array di interi. Il programma dovrebbe creare due thread, ognuno dei quali calcolerà la 
   somma parziale di una metà dell'array. Infine, il thread principale dovrà combinare le due somme 
   parziali per ottenere la somma totale dell'intero array.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define size 10

int somma_parziale = 0; // Variabile per la somma parziale del primo thread
int somma_parziale2 = 0; // Variabile per la somma parziale del secondo thread

// Funzione eseguita dal primo thread
void *routine(void *arg)
{
    int *array = (int *)arg; // Casting dell'argomento a un puntatore a un array di interi
    for (int i = 0; i < size / 2; i++)
    {
        somma_parziale += array[i]; // Calcolo della somma parziale della prima metà dell'array
    }
    printf("\nLa prima somma parziale è: %d ", somma_parziale);
    pthread_exit(NULL); // Terminazione del thread
}

// Funzione eseguita dal secondo thread
void *routine2(void *arg)
{
    int *array = (int *)arg; // Casting dell'argomento a un puntatore a un array di interi
    for (int i = size / 2; i < size; i++)
    {
        somma_parziale2 += array[i]; // Calcolo della somma parziale della seconda metà dell'array
    }
    printf("\nLa seconda somma parziale è: %d ", somma_parziale2);
    pthread_exit(NULL); // Terminazione del thread
}

int main()
{
    int array[size] = {25, 30, 87, 11, 10, 2, 4, 23, 9, 7};

    pthread_t th1; // Identificatore del primo thread
    pthread_t th2; // Identificatore del secondo thread
    int somma_finale = 0; // Variabile per la somma finale dell'intero array

    // Creazione dei Thread
    if (pthread_create(&th1, NULL, routine, (void *)array) != 0)
    {
        printf("\nErrore nella creazione del thread");
        return -1;
    }

    if (pthread_create(&th2, NULL, routine2, (void *)array) != 0)
    {
        printf("\nErrore nella creazione del thread");
        return -1;
    }

    // Fase di JOIN
    if (pthread_join(th1, NULL) != 0)
    {
        printf("\nErrore nella fase di join");
        return -2;
    }

    if (pthread_join(th2, NULL) != 0)
    {
        printf("\nErrore nella fase di join");
        return -2;
    }

    somma_finale = somma_parziale + somma_parziale2; // Calcolo della somma finale dell'intero array

    printf("\nLa somma finale è: %d \n", somma_finale);

    return 0;
}