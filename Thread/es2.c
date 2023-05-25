/* Scrivere un programma che accetta un intero n da riga di comando, 
   crea n thread e poi aspetta la loro terminazione
    • Ciascun thread aspetta un numero di secondi casuale tra 1 e 10, 
      poi incrementa una variabile globale intera ed infine ne stampa 
      il valore
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define n 4

int globalVariable = 0;

//Dichiaro un MUTEX perchè lavoreremo su una variabile globale
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void* threadFunction(void* arg) {
    //Genero un numero randomico da 1 a 10 per poi passarlo alla sleep
    int randomSeconds = rand() % 1 + 1;
    printf("Il thread %ld ha aspettato %d secondi", pthread_self(), randomSeconds);
    sleep(randomSeconds);

    //Incremento in mutua esclusione la variabile globale
    //pthread_mutex_lock(&mutex);
        globalVariable++;
        printf("\nValore incrementato: %d\n", globalVariable);
    //pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    
    /* Questo posso usarlo solo se lo runno da terminale
        printf("\nInserire il valore di N:");
        int n = atoi(argv[1]);
    */
    srand(time(NULL));
    pthread_t* threads = (pthread_t*)malloc(n * sizeof(pthread_t));

    if (threads == NULL) {
        printf("Errore nell'allocazione della memoria per i thread\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (pthread_create(&threads[i], NULL, threadFunction, NULL) != 0) {
            printf("Errore nella creazione dei thread\n");
            return 1;
        }
        pthread_join(threads[i], NULL);
    }



    // for (int i = 0; i < n; i++) {
    //     if (pthread_join(threads[i], NULL) != 0) {
    //         printf("Errore nell'attesa dei thread\n");
    //         return 1;
    //     }
    // }

    // free(threads);

    return 0;
}

