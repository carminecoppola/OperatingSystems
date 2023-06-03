/* Si realizzi un programma in C e Posix sotto Linux che, con l'ausilio della libreria Pthread,
   lancia m thread per calcolare la somma dei prodotti delle colonne di due matrici mxm.
   L'i-esimo thread, dopo aver calcolato la somma dei prodotti delle colonne i-esime delle due
   matrici, inserisce il risultato in un array m-dimensionale, in modo concorrente, nella prima
   locazione libera disponibile. Non appena l'array sarà riempito, un m+1-esimo thread, che era
   in attesa, provvederà a stampare il contenuto. Le matrici devono essere allocate dinamicamente.
   Usare mutex e variabili di condizione. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define m 4
#define maxMatrix 20

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex per la sincronizzazione dei thread
pthread_cond_t varCond = PTHREAD_COND_INITIALIZER;     // Variabile di condizione per la sincronizzazione dei thread

/*  Le variabili globali matrix1 e matrix2 sono dichiarate all'inizio del programma e sono accessibili
    da tutte le funzioni. Questo ci permette di allocare e inizializzare le matrici nel blocco di codice
    principale (main) e di liberare la memoria alla fine del programma. Inoltre, poiché i thread devono
    accedere alle matrici per calcolare i risultati, l'accesso alle variabili globali consente loro di 
    ottenere facilmente i dati necessari.   */

int **matrix1;         // Prima matrice
int **matrix2;         // Seconda matrice
int results[m];        // Array dei risultati
int lastPosition = 0;  // Ultima posizione occupata nell'array dei risultati

/*  La struttura ThreadData viene utilizzata per passare i dati ai thread creati,
    necessari per il calcolo dei risultati.Essa contiene due puntatori alle matrici
    matrix1 e matrix2 e un intero column che rappresenta l'indice della colonna che
    il thread deve elaborare.    */

typedef struct threadD {
    int **matrix1;
    int **matrix2;
    int column;
} ThreadData;

// Funzione per l'allocazione dinamica di una matrice
void allocationMatrix(int ***matrix) {
    *matrix = (int **)calloc(m, sizeof(int *));
    for (int i = 0; i < m; i++)
        (*matrix)[i] = (int *)calloc(m, sizeof(int));
}

// Funzione per l'inizializzazione casuale dei valori di una matrice
void initMatrix(int **matrix) {
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            matrix[i][j] = rand() % maxMatrix + 1;
}

// Funzione per la stampa di una matrice
void printMatrix(int **matrix) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            printf(" %d ",matrix[i][j]);
        }
        printf("\n");
    }
}

// Funzione per la deallocazione di una matrice
void deallocationMatrix(int **matrix) {
    for (int i = 0; i < m; i++)
        free(matrix[i]);
    free(matrix);
}

// Funzione eseguita da ogni thread
void *routine(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int sumP = 0;

    // Calcolo della somma dei prodotti delle colonne i-esime delle due matrici
    for (int i = 0; i < m; i++) {
        sumP += data->matrix1[i][data->column] * data->matrix2[i][data->column];
    }

    // Blocco l'accesso al mutex per modificare l'array dei risultati in modo sicuro
    pthread_mutex_lock(&myMutex);
        results[lastPosition] = sumP;  // Inserisco il risultato nella prossima posizione libera dell'array
        lastPosition++;                // Incremento l'ultima posizione occupata nell'array
    pthread_mutex_unlock(&myMutex);  // Sblocco l'accesso al mutex

    // Se ho completato l'inserimento di tutti i risultati, segnalo il thread di stampa dei risultati
    if (lastPosition == m) {
        pthread_cond_signal(&varCond);
    }

    free(data);  // Libero la memoria allocata per la struttura ThreadData
    pthread_exit(NULL);
}

// Funzione eseguita dal thread di stampa dei risultati
void *stampaArrayRisultati(void *arg) {
    pthread_mutex_lock(&myMutex);
    if (lastPosition < m) {
        pthread_cond_wait(&varCond, &myMutex);  // Il thread si mette in attesa finché non sono disponibili tutti i risultati
    }
    printf("\nArray dei risultati:\n");
    printf("[");
    for (int i = 0; i < m; i++) {
        printf(" %d,",results[i]);
    }
    printf("]\n");
    pthread_mutex_unlock(&myMutex);  // Sblocco l'accesso al mutex

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    pthread_t th[m];       // Thread per i calcoli
    pthread_t resultTh;    // Thread per la stampa dei risultati

    allocationMatrix(&matrix1);  // Alloco dinamicamente la prima matrice
    allocationMatrix(&matrix2);  // Alloco dinamicamente la seconda matrice

    printf("\nQuesta è la prima matrice:\n");
    initMatrix(matrix1);    // Inizializzo casualmente i valori della prima matrice
    printMatrix(matrix1);   // Stampo la prima matrice

    printf("\nQuesta è la seconda matrice:\n");
    initMatrix(matrix2);    // Inizializzo casualmente i valori della seconda matrice
    printMatrix(matrix2);   // Stampo la seconda matrice

    // Creazione dei thread per il calcolo dei risultati
    for (int i = 0; i < m; i++) {
        // Allocazione della memoria per la struttura ThreadData
        ThreadData *data = (ThreadData *)malloc(sizeof(ThreadData));

        // Assegnazione dei puntatori alle matrici e dell'indice della colonna
        data->matrix1 = matrix1;  // Assegna il puntatore alla prima matrice
        data->matrix2 = matrix2;  // Assegna il puntatore alla seconda matrice
        data->column = i;  // Assegna l'indice della colonna corrente

        // Creazione del thread
        if (pthread_create(&th[i], NULL, routine, data) != 0) {
            printf("\nErrore nella creazione del thread");
            return -1;
        }
        // Il thread creato eseguirà la funzione 'routine' con i dati specifici
        // della colonna da elaborare contenuti nella struttura 'data'
    }


    // Creazione del thread per la stampa dei risultati
    if (pthread_create(&resultTh, NULL, stampaArrayRisultati, NULL) != 0) {
        printf("\nErrore nella creazione del thread di stampa");
        return -1;
    }

    // Join dei thread per attendere la loro terminazione
    for (int i = 0; i < m; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            printf("\nErrore nella fase di JOIN del thread");
            return -2;
        }
    }

    if (pthread_join(resultTh, NULL) != 0) {
        printf("\nErrore nella fase di JOIN del thread di stampa");
        return -2;
    }

    // Deallocazione delle matrici
    deallocationMatrix(matrix1);
    deallocationMatrix(matrix2);

    return 0;
}
