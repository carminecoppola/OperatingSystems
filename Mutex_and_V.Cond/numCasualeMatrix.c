/*  Da data una matrice nxn( con n pari) di interi generati casualmente, allocata dinamicamente, 
    con n argomento da riga di comando, creare n thread che prelevano casualmente un elemento 
    della riga di competenza (thread i-esimo, riga i-esima) e lo inseriscano concorrentemente 
    in un vettore di (n + 1)/2 elementi.
    Un thread n + 1 - esimo attende il riempimento del vettore per stampare il contenuto dello
    stesso e per stampare il numero di elementi inseriti nel vettore da ciascun thread. 
    Usare mutex e variabili di condizione   */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int n;                      // Dimensione della matrice nxn
int **matrix;               // Puntatore alla matrice
int *vettore;               // Puntatore al vettore
int contElement = 0;        // Contatore per il numero di elementi inseriti nel vettore
int threadCount = 0;        // Contatore per il numero di thread creati

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;   // Mutex per la sincronizzazione
pthread_cond_t vettoreRiempito = PTHREAD_COND_INITIALIZER;

void allocaMatrix()
{
    matrix = (int **)malloc(n * sizeof(int *));        // Allocazione dinamica della matrice
    for (int i = 0; i < n; i++)
    {
        matrix[i] = (int *)malloc(n * sizeof(int));
    }
}

void deallocaMatrix()
{
    for (int i = 0; i < n; i++)
    {
        free(matrix[i]);       // Liberazione della memoria della matrice
    }
    free(matrix);
}

void generaMatrix()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = rand() % 9 + 1;        // Generazione casuale degli elementi della matrice
        }
    }
}

void printMatrix()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf(" %d ", matrix[i][j]);         // Stampa degli elementi della matrice
        }
        printf("\n");
    }
}

void *routinePrelievo(void *arg)
{
    int threadId = *((int *)arg);

    for (int i = threadId; i < n; i ++)
    {
        for (int j = 0; j < n; j++)
        {
            if(contElement < (n + 1)/2){
                pthread_mutex_lock(&myMutex);      // Acquisizione del mutex per la sincronizzazione
                    vettore[contElement] = matrix[i][j];   // Inserimento dell'elemento nel vettore
                    contElement++;                  // Incremento del contatore degli elementi inseriti
                pthread_mutex_unlock(&myMutex);    // Rilascio del mutex
            }
        }
    }

    pthread_mutex_lock(&myMutex);
        if (contElement >= (n + 1) / 2) {
            pthread_cond_signal(&vettoreRiempito);
        }
    pthread_mutex_unlock(&myMutex);

    pthread_exit(NULL);
}

void *routineStampa(void *arg)
{
    int size = (n + 1) / 2;

    pthread_mutex_lock( &myMutex );
        while (contElement < (n + 1) / 2) {
            pthread_cond_wait(&vettoreRiempito, &myMutex);
        }
        printf("\n\nSono il thread di stampa, ecco il Vettore di size %d:\n", size);
        for (int i = 0; i < size; i++)
        {
            printf(" %d ", vettore[i]);      // Stampa degli elementi del vettore
        }
        printf("\n");
    pthread_mutex_unlock( &myMutex );

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    n = atoi(argv[1]);          // Dimensione della matrice passata come argomento da riga di comando

    allocaMatrix();             // Allocazione della matrice
    generaMatrix();             // Generazione casuale degli elementi della matrice

    printf("\nQuesta è la tua matrice di dimensione %dx%d:\n", n, n);
    printMatrix();              // Stampa della matrice

    vettore = (int *)malloc(((n + 1) / 2) * sizeof(int));    // Allocazione del vettore

    pthread_t th[n], printTh;
    int threadIds[n];

    for (int i = 0; i < n; i++)
    {
        threadIds[i] = i;
        if (pthread_create(&th[i], NULL, routinePrelievo, &threadIds[i]) != 0)
        {
            printf("\nErrore nella creazione dei thread");       // Creazione dei thread per la routine di prelievo
            return -1;
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            printf("\nErrore nella fase di JOIN dei thread");     // Join dei thread per attendere la loro terminazione
            return -2;
        }
    }

    if (pthread_create(&printTh, NULL, routineStampa, NULL) != 0)
    {
        printf("\nErrore nella creazione del thread di stampa");   // Creazione del thread per la routine di stampa
        return -1;
    }

    if (pthread_join(printTh, NULL) != 0)
    {
        printf("\nErrore nella fase di JOIN del thread di stampa");   // Join del thread di stampa
        return -2;
    }

    deallocaMatrix();           // Deallocazione della matrice
    free(vettore);              // Liberazione della memoria del vettore

    return 0;
}
