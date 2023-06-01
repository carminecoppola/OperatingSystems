/*  Si realizzi un programma in C e Posix sotto Linux che, utilizzando la libreria Pthread:
    • Lancia n thread per cercare un elemento in una matrice NxN di interi;
    • Ognuno dei thread cerca l'elemento in una delle righe della matrice;
    • Non appena un thread ha trovato l'elemento cercato, rende note agli 
      altri thread le coordinate dell'elemento e tutti i thread terminano 
      (sono cancellati)
*/


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

#define DIM_M 4

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
int trovato = 0;

int matrix[DIM_M][DIM_M]; // La dichiaro globale per comodità

void generateMatrix(int matrix[DIM_M][DIM_M])
{
    for (int i = 0; i < DIM_M; i++)
    {
        for (int j = 0; j < DIM_M; j++)
        {
            matrix[i][j] = rand() % 9 + 1; // Genera un numero casuale da 1 a 9
        }
    }
}

void printMatrix(int matrix[DIM_M][DIM_M])
{
    for (int i = 0; i < DIM_M; i++)
    {
        for (int j = 0; j < DIM_M; j++)
        {
            printf(" %d ", matrix[i][j]); // Stampa gli elementi della matrice
        }
        printf("\n");
    }
}

void *searchNumber(void *arg)
{
    int element = *(int*)arg;

    for (int i = 0; i < DIM_M; i++)
    {
        for (int j = 0; j < DIM_M; j++)
        {
            pthread_mutex_lock(&myMutex); // Blocca il mutex per l'accesso in mutua esclusione
            if (matrix[i][j] == element && !trovato) // Controlla se l'elemento è stato trovato e se non è già stato segnalato
            {
                printf("\n**L'elemento è stato trovato nella posizione [%d][%d]**\n", i, j);
                trovato = 1; // Imposta la variabile trovato a 1 per segnalare che l'elemento è stato trovato
            }
            pthread_mutex_unlock(&myMutex); // Sblocca il mutex

            if (trovato)
            {
                pthread_exit(NULL); // Termina il thread se l'elemento è stato trovato
            }
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    int element = atoi(argv[1]);

    // Metto un controllo poiché la matrice contiene numeri da 1 a 9
    if (element > 9)
    {
        printf("\nATTENZIONE: la matrice contiene numeri da 1 a 9\n");
        return -1;
    }

    generateMatrix(matrix);

    int n = 0;

    printf("\nInserisci il numero di thread da creare:\n");
    scanf("%d", &n);

    if (n <= 0)
    {
        printf("\nIl numero di thread deve essere maggiore di zero.\n");
        return -3;
    }

    pthread_t th[n];

    for (int i = 0; i < n; i++)
    {
        if (pthread_create(&th[i], NULL, searchNumber, &element) != 0) // Crea i thread
        {
            printf("\nNon è stato possibile creare i thread\n");
            return -1;
        }
    }

    for (int i = 0; i < n; i++)
    {
        pthread_join(th[i], NULL); // Attende la terminazione dei thread
    }

    //Controllo se la variabile trovato è ancora a 0 significa che non è stato trovato l'elemento
    if (!trovato)
    {
        printf("\nL'elemento %d non è contenuto nella matrice.\n", element);
    }

    printf("\nEcco la matrice:\n");
    printMatrix(matrix);

    return 0;
}
