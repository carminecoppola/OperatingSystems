/*  Scrivere un programma C e Posix sotto Linux che, utilizzando la libreria Pthread crei 
    due Thread "somma1" e "somma2",entrambi accedono alle variabili test.a e test.b di una 
    struttura dati test condivisa incrementandole di 1 per 10 volte. Si utilizzi come meccanismo
    di gestione della concorrenza quello della "mutua esclusione per struttura", dove la struttura 
    mutex va allocata dinamicamente. 
    
    Esempio di Output:
    Iterazione 0 del Thread somma1 --> test.a = 1, test.b = 1
    Iterazione 1 del Thread somma1 --> test.a = 3, test.b = 3
    Iterazione 2 del Thread somma1 --> test.a = 4, test.b = 4
    Iterazione 3 del Thread somma1 --> test.a = 5, test.b = 5
    Iterazione 4 del Thread somma1 --> test.a = 6, test.b = 6
    Iterazione 5 del Thread somma1 --> test.a = 7, test.b = 7
    Iterazione 6 del Thread somma1 --> test.a = 8, test.b = 8
    Iterazione 7 del Thread somma1 --> test.a = 9, test.b = 9
    Iterazione 8 del Thread somma1 --> test.a = 10, test.b = 10
    Iterazione 9 del Thread somma1 --> test.a = 11, test.b = 11

    Iterazione 0 del Thread somma2 --> test.a = 2, test.b = 2
    Iterazione 1 del Thread somma2 --> test.a = 12, test.b = 12
    Iterazione 2 del Thread somma2 --> test.a = 13, test.b = 13
    Iterazione 3 del Thread somma2 --> test.a = 14, test.b = 14
    Iterazione 4 del Thread somma2 --> test.a = 15, test.b = 15
    Iterazione 5 del Thread somma2 --> test.a = 16, test.b = 16
    Iterazione 6 del Thread somma2 --> test.a = 17, test.b = 17
    Iterazione 7 del Thread somma2 --> test.a = 18, test.b = 18
    Iterazione 8 del Thread somma2 --> test.a = 19, test.b = 19
    Iterazione 9 del Thread somma2 --> test.a = 20, test.b = 20

    */



#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct dati {
    int test_a;
    int test_b;
    pthread_mutex_t myMutex;
} test;

// Funzione eseguita dal thread somma1
void *somma1(void *arg) {
    test *puntatore = (test*) arg;

    // Ciclo di incremento delle variabili per 10 volte
    for (int i = 0; i < 10; i++) {
        // Acquisisce il mutex per garantire l'accesso esclusivo alle variabili condivise
        pthread_mutex_lock(&(puntatore->myMutex));
        
            // Incrementa le variabili test_a e test_b all'interno del blocco protetto dal mutex
            puntatore->test_a++;
            puntatore->test_b++;
        
        // Rilascia il mutex per permettere ad altri thread di accedere alle variabili condivise
        pthread_mutex_unlock(&(puntatore->myMutex));
        
        // Stampa l'output che mostra i valori delle variabili dopo l'incremento
        printf("Iterazione %d del Thread somma1 --> test.a = %d, test.b = %d\n", i, puntatore->test_a, puntatore->test_b);
    }
    
    printf("\n");
    
    // Termina il thread
    pthread_exit(NULL);
}

// Funzione eseguita dal thread somma2
void *somma2(void *arg) {
    test *puntatore = (test*) arg;

    // Ciclo di incremento delle variabili per 10 volte
    for (int i = 0; i < 10; i++) {
        // Acquisisce il mutex per garantire l'accesso esclusivo alle variabili condivise
        pthread_mutex_lock(&(puntatore->myMutex));
        
            // Incrementa le variabili test_a e test_b all'interno del blocco protetto dal mutex
            puntatore->test_a++;
            puntatore->test_b++;
        
        // Rilascia il mutex per permettere ad altri thread di accedere alle variabili condivise
        pthread_mutex_unlock(&(puntatore->myMutex));
        
        // Stampa l'output che mostra i valori delle variabili dopo l'incremento
        printf("Iterazione %d del Thread somma2 --> test.a = %d, test.b = %d\n", i, puntatore->test_a, puntatore->test_b);
    }

    // Termina il thread
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    // Alloca la memoria per la struttura dati condivisa
    test *datiCondivisi = (test*) malloc(sizeof(test));

    // Inizializza le variabili test_a e test_b a 0
    datiCondivisi->test_a = 0;
    datiCondivisi->test_b = 0;

    // Inizializza il mutex per la gestione della concorrenza
    pthread_mutex_init(&(datiCondivisi->myMutex), NULL);

    pthread_t t_somma1, t_somma2;

    // Creazione dei thread somma1 e somma2
    if (pthread_create(&t_somma1, NULL, somma1, (void*) datiCondivisi) != 0) {
        printf("\nErrore nella creazione del thread somma1\n");
        return -1;
    }

    if (pthread_create(&t_somma2, NULL, somma2, (void*) datiCondivisi) != 0) {
        printf("\nErrore nella creazione del thread somma2\n");
        return -1;
    }

    // Attende la terminazione dei thread somma1 e somma2
    if (pthread_join(t_somma1, NULL) != 0) {
        printf("\nErrore nella fase di JOIN del thread somma1\n");
        return -2;
    }

    if (pthread_join(t_somma2, NULL) != 0) {
        printf("\nErrore nella fase di JOIN del thread somma2\n");
        return -2;
    }

    // Distrugge il mutex e libera la memoria allocata per la struttura dati
    pthread_mutex_destroy(&(datiCondivisi->myMutex));
    free(datiCondivisi);

    return 0;
}
