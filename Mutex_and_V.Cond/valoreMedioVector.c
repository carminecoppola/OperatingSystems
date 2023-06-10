/*  Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria Pthread,
    crei un vettore di interi di dimensione n, dove n>=21 e multiplo di 3, è un argomento 
    passato da riga di comando. Calcolato il valore medio degli elementi contenuti nel vettore,
    il programma deve determinare, impiegando tre thread, il numero di elementi maggiori
    del valore medio, rispettivamente nei tre sottovettori di dimensioni n/3.   */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int n = 0;
int valMedio;  // Valore medio degli elementi del vettore
int *contatori;  // Array dei contatori per i thread
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex per sincronizzazione

// Calcola il valore medio degli elementi nel vettore
int valoreMedio(int *vector)
{
    int somma = 0;

    // Calcola la somma di tutti gli elementi del vettore
    for (int i = 0; i < n; i++)
    {
        somma += vector[i];
    }

    valMedio = somma / n;  // Calcola il valore medio
    printf("\nLa somma è: %d", somma);
    printf("\nIl valore medio è: %d", valMedio);
    return valMedio;
}

// Routine eseguita dai thread
void *routine(void *arg)
{
    int *vector = (int *)arg;
    int contatore = 0;

    // Conta il numero di elementi maggiori del valore medio nel sottovettore
    for (int i = 0; i < n / 3; i++)
    {
        if (vector[i] > valMedio)
        {
            printf("\nIl valore [%d], nella posizione [%d] è maggiore del valore medio %d\n", vector[i], i, valMedio);
            contatore++;
        }
    }

    pthread_mutex_lock(&mutex);  // Acquisisce il lock del mutex
    *contatori += contatore;  // Aggiorna il contatore globale
    pthread_mutex_unlock(&mutex);  // Rilascia il lock del mutex

    pthread_exit(NULL);  // Termina il thread
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (argc != 2)
    {
        printf("\nUsage: ./program <n>\n");
        return 1;
    }

    n = atoi(argv[1]);

    if (n < 21 || n % 3 != 0)
    {
        printf("\nErrore: N deve essere maggiore di 21 e multiplo di 3\n");
        exit(0);
    }

    int *vettore = malloc(sizeof(int) * n);

    // Inizializza il vettore con valori casuali
    for (int i = 0; i < n; i++)
    {
        vettore[i] = rand() % 10 + 1;
    }

    printf("\nEcco il vettore iniziale:");
    for (int i = 0; i < n; i++)
    {
        printf(" %d ", vettore[i]);
    }
    printf("\n");

    valMedio = valoreMedio(vettore);  // Calcola il valore medio del vettore

    pthread_t th[3];
    contatori = malloc(sizeof(int));

    for (int i = 0; i < 3; i++)
    {
        if (pthread_create(&th[i], NULL, routine, (void *)vettore) != 0)
        {
            printf("\nErrore nella creazione dei thread");
            return -1;
        }
    }

    // JOIN
    for (int i = 0; i < 3; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            printf("\nErrore nella fase di JOIN dei thread");
            return -2;
        }
    }

    printf("\nIl numero di elementi maggiori del valore medio è: %d\n", *contatori);

    free(vettore);
    free(contatori);

    return 0;
}
