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

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

#define SIZE 4

int** matrix;             // Dichiarazione di una matrice di tipo puntatore a puntatore di interi.
int somma_totale[SIZE];   // Dichiarazione di un array globale 'somma_totale' di interi di dimensione SIZE.

// Crea e alloca dinamicamente la matrice casuale da 0 a 255.
int** generaMatrice()
{
    matrix = (int**)malloc(SIZE * sizeof(int*));   // Alloca memoria per le righe della matrice.

    for (int i = 0; i < SIZE; i++)
    {
        matrix[i] = (int*)malloc(SIZE * sizeof(int));    // Alloca memoria per le colonne di ciascuna riga.

        for (int j = 0; j < SIZE; j++)
        {
            matrix[i][j] = rand() % 255 + 1;   // Genera un numero casuale compreso tra 0 e 255 e lo assegna all'elemento corrispondente nella matrice.
        } 
    }
    return matrix;   // Restituisce il puntatore alla matrice generata.
}

// Stampa la matrice.
void stampaMatrice(int** matrix)
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf(" %d ", matrix[i][j]);   // Stampa gli elementi della matrice.
        }
        printf("\n");
    }
}

// Funzione eseguita dai thread.
void* routine(void *arg)
{
    int indice = *(int*)arg;   // Ottiene l'indice del thread passato come argomento.

    int somma = 0;  //Mi serve come variabile per indicare la somma parziale
    
    // Se ha l'indice pari
    if(indice % 2 == 0)
    {
        for (int i = 0; i < SIZE; i += 2)
        {
            somma += matrix[indice][i];   // Somma gli elementi di indice pari della riga corrispondente.
        }
        printf("\nLa somma parziale (pari) del Thread[%d] è: %d",indice ,somma);
    }
    // Se ha l'indice dispari
    else
    {
        for (int i = 1; i < SIZE; i += 2)
        {
            somma += matrix[indice][i];   // Somma gli elementi di indice dispari della riga corrispondente.
        }
        printf("\nLa somma parziale (dispari) del Thread[%d] è: %d", indice, somma);
    }

    somma_totale[indice] = somma;   // Memorizza la somma parziale nell'array somma_totale.
    pthread_exit(NULL);   // Termina l'esecuzione del thread.
}

// Trova il valore minimo nell'array.
int minimo(int* array)
{
    int min = array[0];   // Assume il primo elemento come minimo iniziale.

    for (int i = 1; i < SIZE; i++)
    {
        if (array[i] < min)
            min = array[i];   // Se trova un elemento minore, lo aggiorna come nuovo minimo.
    }
    return min;   // Restituisce il valore minimo trovato.
}

int main(int argc, char *args[])
{
    srand(time(NULL));   // Inizializza il generatore di numeri casuali con il valore del tempo corrente come seme.

    matrix = generaMatrice();   // Genera la matrice casuale.

    printf("\nData questa matrice:\n");
    stampaMatrice(matrix);   // Stampa la matrice.

    pthread_t th[SIZE];   // Dichiarazione di un array di thread di dimensione SIZE.
    int indici[SIZE];     // Dichiarazione di un array di interi di dimensione SIZE per memorizzare gli indici dei thread.

    // Creazione dei thread.
    for (int i = 0; i < SIZE; i++)
    {
        indici[i] = i;   // Assegna l'indice corrente all'array degli indici.

        if (pthread_create(&th[i], NULL, routine, (void*)&indici[i]) != 0)
        {
            printf("\nErrore nella creazione dei thread\n");
            return -1;
        }
        
    }

    int result[SIZE];

    // Join dei thread.
    for (int i = 0; i < SIZE; i++)
    {
        if (pthread_join(th[i], (void**)&result[i]) != 0)
        {
            printf("\nErrore nella fase di JOIN dei thread\n");
            return -2;
        }
        
    }

    int min = minimo(somma_totale);   // Trova il valore minimo nell'array delle somme parziali.
    printf("\n\nIl minimo tra le somme parziali è: %d\n", min);    // Stampa il valore minimo.

    // Libera la memoria della matrice.
    for (int i = 0; i < SIZE; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
