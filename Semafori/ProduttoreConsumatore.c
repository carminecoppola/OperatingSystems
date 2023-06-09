/*  Scrivere un programma C in ambiente Linux che facendo uso della libreria Pthread e dei 
    semafori POSIX realizzi il seguente comportamento:
        • Ci sono due thread produttori e due thread consumatori;
        • C'è una risorsa condivisa (un buffer) di dimensione N;
    I Produttori scrivono valori interi casuali nel buffer e i Consumatori leggono i valori dal buffer.
    Il programma deve garantire che:
        • I Produttori non scrivano nel buffer quando è pieno.
        • I Consumatori non leggano dal buffer quando è vuoto.
        • Non ci siano conflitti nell'accesso al buffer tra i produttori e i consumatori.
    Utilizzare semafori POSIX per garantire la sincronizzazione tra i thread.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include<unistd.h>

#define N 5

sem_t empty, full, mutex;
int countElement = 0;

void* routineProduttori(void* arg)
{
    int *buff = (int*)arg;
    int indice = 0;
    int randNum;

    while (1)
    {
        randNum = rand() % 1000 + 1;

        sem_wait(&empty); // Aspetto che ci sia almeno un posto vuoto nel buffer

        sem_wait(&mutex); // Blocco l'accesso al buffer con un semaforo mutex
        buff[indice] = randNum;
        printf("\nL'elemento generato è: [%d]", randNum);
        indice = (indice + 1) % N;
        countElement++;
        printf("\nL'indice è: %d", indice);
        printf("\nGli elementi attuali sono: %d", countElement);
        sem_post(&mutex);

        sem_post(&full);

        if (countElement == N - 1)
            sem_post(&empty); // Sveglio un consumatore se era in attesa

        sleep(1); // Aggiungiamo un ritardo per rendere l'esecuzione più chiara
    }

    pthread_exit(NULL);
}

void* routineConsumatori(void* arg)
{
    int *buff = (int*)arg;
    int indice = 0;

    while (1)
    {
        sem_wait(&full); // Aspetto che ci sia almeno un elemento nel buffer

        sem_wait(&mutex); // Blocco l'accesso al buffer con un semaforo mutex
        int value = buff[indice];
        printf("\nL'elemento letto è: [%d]", value);
        buff[indice] = 0; // Inizializzo la posizione appena letta e quindi consumata
        indice = (indice + 1) % N;
        countElement--;
        printf("\nGli elementi attuali sono: %d", countElement);
        sem_post(&mutex);

        sem_post(&empty);

        if (countElement == 0)
            sem_post(&full); // Sveglio un produttore se era in attesa

        sleep(1); // Aggiungiamo un ritardo per rendere l'esecuzione più chiara
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    srand(time(NULL));

    int buffer[N];

    sem_init(&empty, 0, N);   // Inizializzo il semaforo empty al numero massimo di posti vuoti nel buffer
    sem_init(&full, 0, 0);    // Inizializzo il semaforo full a 0
    sem_init(&mutex, 0, 1);   // Inizializzo il semaforo mutex a 1

    pthread_t thProduttori[2], thConsumatori[2];

    for (int i = 0; i < 2; i++)
    {
        if (pthread_create(&thProduttori[i], NULL, routineProduttori, (void*)buffer) != 0)
        {
            printf("\nErrore nella creazione dei thread Produttori");
            return -1;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        if (pthread_create(&thConsumatori[i], NULL, routineConsumatori, (void*)buffer) != 0)
        {
            printf("\nErrore nella creazione dei thread Consumatori");
            return -1;
        }
    }

    // Fase di JOIN
    for (int i = 0; i < 2; i++)
    {
        if (pthread_join(thProduttori[i], NULL) != 0)
        {
            printf("\nErrore nella fase di JOIN dei thread Produttori");
            return -2;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        if (pthread_join(thConsumatori[i], NULL) != 0)
        {
            printf("\nErrore nella fase di JOIN dei thread Consumatori");
            return -2;
        }
    }

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
