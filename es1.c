/*  Realizzare un programma C e Posix sotto Linux che sincronizzi un thread padre ed un thread figlio
    che scrivono e leggono, rispettivamente, un numero intero alla volta (da 1 a n, dove n è passato da 
    riga di comando) nella prima posizione di un file temporaneo opportunamente creato.
    Implementare il programma con i processi ed i semafori Posix.   */


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int n = 0;


void *routinePadre( void* arg)
{



}

void *routineFiglio( void* arg)
{



}




int main(int argc, char* argv[])
{
    
    n = atoi(argv[1]);

    pthread_t thDad, thSon;

    if (pthread_create( &thDad, NULL, routinePadre, NULL) != 0)
    {
        printf("\nErrore nella creazione del thread padre");
        return -1;
    }

    if (pthread_create( &thSon, NULL, routineFiglio, NULL) != 0)
    {
        printf("\nErrore nella creazione del thread figlio");
        return -1;
    }


    if (pthread_join( thDad, NULL) != 0)
    {
        printf("\nErrore nella fase di JOIN del thread padre");
        return -2;
    }

    if (pthread_join( thSon, NULL) != 0)
    {
        printf("\nErrore nella fase di JOIN del thread figlio");
        return -2;
    }

    


    return 0;
}