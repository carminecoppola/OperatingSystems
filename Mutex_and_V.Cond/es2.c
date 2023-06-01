/*  Si realizzi un programma in C e Posix sotto Linux che, con l'ausilio della libreria Pthread
        • Lancia n thread per calcolare la somma degli elementi di ciascuna riga di una matrice 
          nxn di interi generati casualmente in un intervallo [0,255], allocata dinamicamente;
        • Il calcolo della somma degli elementi di ciascuna riga deve essere effettuato 
          concorrentemente su tutte le righe, secondo la seguente modalità:
             • il thread i-esimo con i pari, calcola la somma degli elementi di indice pari 
               della riga i-esima;
             • il thread con indice i dispari, calcola la somma degli elementi di indice 
               dispari della riga i-esima;
        • Calcolate le somme parziali, si provvederà a ricercarne il minimo ed a stamparlo a video;
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 4

int** matrix;
int somma_totale[SIZE];

int** generaMatrice()
{
    int** matrix = (int**)malloc(SIZE * sizeof(int*));

    for (int i = 0; i < SIZE; i++)
    {
        matrix[i] = (int*)malloc(SIZE * sizeof(int));
        for (int j = 0; j < SIZE; j++)
        {
            matrix[i][j] = rand() % 256; // Genera un numero casuale tra 0 e 255
        }
    }
    return matrix;
}

void stampaMatrice(int** matrix)
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf(" %d ", matrix[i][j]); // Stampa gli elementi della matrice
        }
        printf("\n");
    }
}

void* routine(void* arg)
{
    int indice = *(int*)arg;

    int somma = 0;

    if (indice % 2 == 0)
    {
        for (int i = 0; i < SIZE; i += 2)
        {
            somma += matrix[indice][i];
        }
    }
    else
    {
        for (int i = 1; i < SIZE; i += 2)
        {
            somma += matrix[indice][i];
        }
    }

    somma_totale[indice] = somma;
    pthread_exit(NULL);
}

int minimo(int* array)
{
    int min = array[0];

    for (int i = 1; i < SIZE; i++)
    {
        if (array[i] < min)
            min = array[i];
    }
    return min;
}

int main(int argc, char* args[])
{
    srand(time(NULL));

    matrix = generaMatrice();

    stampaMatrice(matrix);

    pthread_t th[SIZE];
    int indici[SIZE];

    for (int i = 0; i < SIZE; i++)
    {
        indici[i] = i;
        if (pthread_create(&th[i], NULL, routine, (void*)&indici[i]) != 0)
        {
            printf("\nErrore nella creazione dei thread\n");
            return -1;
        }
    }

    for (int i = 0; i < SIZE; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            printf("\nErrore nella fase di JOIN dei thread\n");
            return -2;
        }
    }

    int min = minimo(somma_totale);
    printf("\nMinimo: %d\n", min);

    for (int i = 0; i < SIZE; i++)
    {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
