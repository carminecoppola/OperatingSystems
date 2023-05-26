/* Scrivi un programma in C che utilizzi i thread per calcolare la somma degli elementi
   in un array di interi. Il programma dovrebbe creare due thread, ognuno dei quali calcolerà la 
   somma parziale di una metà dell'array. Infine, il thread principale dovrà combinare le due somme 
   parziali per ottenere la somma totale dell'intero array.
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define size 10

int somma_parziale = 0;
int somma_parziale2 = 0;

void *routine(int array[size])
{
    int somma_parziale = 0;
    
    for (int i = 0; i < size/2; i++)
    {
        somma_parziale += array[i];
    }
    printf("\nLa prima somma parziale è: %d ",somma_parziale);

    return somma_parziale;
}

void *routine2(int array[size])
{
    
    for (int i = (size/2) + 1; i < size; i++)
    {
        somma_parziale2 += array[i];
    }
    printf("\nLa seconda somma parziale è: %d ",somma_parziale2);

    return somma_parziale2;
}

int main()
{
    int array[size] = {25,30,87,11,10,2,4,23,9,7};

    pthread_t *th1;
    pthread_t *th2;
    int somma_finale = 0;

    //Creazione dei Thread
    if (pthread_create(&th1, NULL, &routine, NULL) != 0)
    {
        printf("\nErrore nella creazione del thread");
        return -1;
    }
    
    if (pthread_create(&th2, NULL, &routine2, NULL) != 0)
    {
        printf("\nErrore nella creazione del thread");
        return -1;
    }


    //Fase di JOIN
    if(pthread_join(&th1,NULL) != 0)
    {
        printf("\nErrore nella fase di join");
        return -2;
    }
    
    if(pthread_join(&th2,NULL) != 0)
    {
        printf("\nErrore nella fase di join");
        return -2;
    }

    somma_finale = somma_parziale + somma_parziale2;

    printf("\nLa somma finale è: %d",somma_finale);

    return 0;
}