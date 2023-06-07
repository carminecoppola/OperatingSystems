/*  Realizzare un programma C e Posix sotto Linux che sincronizzi un thread padre ed un thread figlio
    che scrivono e leggono, rispettivamente, un numero intero alla volta (da 1 a n, dove n è passato da 
    riga di comando) nella prima posizione di un file temporaneo opportunamente creato.
    Implementare il programma con i processi ed i semafori Posix.   */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int n = 0;
sem_t semPadre;
sem_t semFiglio;

void *routinePadre(void *arg)
{
    FILE *file = (FILE *)arg;

    sem_wait(&semPadre);

    int randomNumber = rand() % n + 1;

    printf("\nHo generato il numero [%d]", randomNumber);

    fprintf(file, "%d\n", randomNumber);

    sem_post(&semFiglio);

    pthread_exit(NULL);
}

void *routineFiglio(void *arg)
{
    FILE *file = (FILE *)arg;

    sem_wait(&semFiglio);

    int readNumero;

    rewind(file);

    fscanf(file, "%d", &readNumero);

    printf("\nIl numero letto è %d\n", readNumero);

    sem_post(&semPadre);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    n = atoi(argv[1]);

    sem_init(&semPadre, 0, 1);
    sem_init(&semFiglio, 0, 0);

    FILE *file = fopen("file.txt", "r+");

    if (file == NULL)
    {
        printf("\nImpossibile aprire il file.\n");
        return 1;
    }

    pthread_t thDad, thSon;

    if (pthread_create(&thDad, NULL, routinePadre, (void *)file) != 0)
    {
        printf("\nErrore nella creazione del thread padre");
        return -1;
    }

    if (pthread_create(&thSon, NULL, routineFiglio, (void *)file) != 0)
    {
        printf("\nErrore nella creazione del thread figlio");
        return -1;
    }

    if (pthread_join(thDad, NULL) != 0)
    {
        printf("\nErrore nella fase di JOIN del thread padre");
        return -2;
    }

    if (pthread_join(thSon, NULL) != 0)
    {
        printf("\nErrore nella fase di JOIN del thread figlio");
        return -2;
    }

    fclose(file);

    sem_destroy(&semPadre);
    sem_destroy(&semFiglio);

    return 0;
}

