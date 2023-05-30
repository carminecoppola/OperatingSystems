/*  Scrivi un programma in C che utilizzi i thread per calcolare il prodotto di due matrici quadrate. 
    Il programma dovrebbe creare un thread per ogni elemento della matrice prodotto. Ogni thread 
    calcolerà il valore di un elemento specifico del prodotto, utilizzando la moltiplicazione di riga
    per colonna tra le matrici di input. Infine, il thread principale dovrà combinare tutti gli 
    elementi calcolati dai thread figli per ottenere la matrice prodotto completa. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 4

int matrix1[N][N];          // Prima matrice di input
int matrix2[N][N];          // Seconda matrice di input
int matrixResult[N][N];     // Matrice risultato del prodotto

// Genera una matrice casuale
void generateMatrix(int matrix[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] = rand() % 10 + 1;    // Genera un numero casuale compreso tra 1 e 10
        }
    }
}

// Stampa una matrice
void printMatrix(int matrix[N][N])
{
    printf("\nEcco la matrice %dx%d:\n", N, N);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf(" %d ", matrix[i][j]);     // Stampa il valore di ogni elemento della matrice
        }
        printf("\n");
    }
}

// Funzione eseguita dai thread figli
void *routine(void *args)
{
    int row = *((int *)args);                  // Indice di riga passato come argomento
    int col = *(((int *)args) + 1);            // Indice di colonna passato come argomento

    int prodotto = 0;

    // Calcolo il prodotto di un elemento specifico
    for (int i = 0; i < N; i++)
    {
        prodotto += matrix1[row][i] * matrix2[i][col];     // Moltiplica gli elementi corrispondenti e li somma
    }
    matrixResult[row][col] = prodotto;

    //Stampo l'id del thread
    printf("Il Thread [%lu] ha calcolato l'elemento (%d,%d)\n", pthread_self(), row, col);

    pthread_exit(NULL);                       // Termina il thread figlio
}

int main()
{
    srand(time(NULL));                        // Inizializza il generatore di numeri casuali con il tempo corrente

    printf("\nPrima Matrice:\n");
    generateMatrix(matrix1);                  // Genera la prima matrice casuale
    printMatrix(matrix1);                     // Stampa la prima matrice

    printf("\nSeconda Matrice:\n");
    generateMatrix(matrix2);                  // Genera la seconda matrice casuale
    printMatrix(matrix2);                     // Stampa la seconda matrice
    printf("\n");

    pthread_t array[N][N];                     // Matrice di thread

    // Creazione dei thread figli per calcolare gli elementi del prodotto
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int *params = malloc(2 * sizeof(int));       // Alloca memoria per i parametri del thread
            params[0] = i;                               // Indice di riga
            params[1] = j;                               // Indice di colonna

            if (pthread_create(&array[i][j], NULL, routine, params) != 0)
            {
                printf("\nErrore nella creazione dei thread");
                return -1;
            }
        }
    }

    // Fase di JOIN per aspettare la terminazione dei thread figli
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (pthread_join(array[i][j], NULL) != 0)
            {
                printf("\nErrore nella fase di JOIN");
                return -2;
            }
        }
    }

    printf("\nEcco la matrice risultante dal prodotto delle due:\n");
    printMatrix(matrixResult);               // Stampa la matrice risultato del prodotto

    return 0;
}
