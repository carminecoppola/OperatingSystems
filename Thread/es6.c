/*  Scrivi un programma in C che utilizzi i thread per calcolare il prodotto di due matrici quadrate. 
    Il programma dovrebbe creare un thread per ogni elemento della matrice prodotto. Ogni thread 
    calcolerà il valore di un elemento specifico del prodotto, utilizzando la moltiplicazione di riga
    per colonna tra le matrici di input. Infine, il thread principale dovrà combinare tutti gli 
    elementi calcolati dai thread figli per ottenere la matrice prodotto completa. */


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

#define N 4

void generateMatrix(int matrix[N][N])
{
    srand(time(NULL));

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix [i][j] = rand() % 10 + 1;
        }
    }
}

void printMatrix(int matrix[N][N])
{

    printf("\nEcco la matrice %dx%d:\n",N,N);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf(" %d ",matrix[i][j]);
        }
        printf("\n");
    }
}

int main(){

    int matrix[N][N];

    generateMatrix(matrix);
    printMatrix(matrix);

    

    return 0;
}