/* Scrivere un programma C che crei 5 thread 
   a cui vengono passati tramite una struttura
   2 parametri, il numero del thread ed un messaggio
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct parametri
{
    int num_thread;
    char messaggio;
};

void *printHello(){

    printf("\nHello world");
}


int main ()
{
    
    struct parametri *params[5];
    int th;

    for (int i = 0; i <= 5; i++)
    {
        printf("Creazione thread %d\n",i);
        th = pthread_create(params[i], NULL, printHello, (void *) params[i]);
    
        if (th) {
            printf("ERROR, return code from pthread_create() is %d\n", th); 
            exit(-1);
        }
    }

    pthread_exit(NULL);


    return 0;
}