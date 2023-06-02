/*  Scrivere un programma C e Posix sotto Linux che, utilizzando la libreria Pthread crei due Thread, 
    chiamati produttore e consumatore. 
    La risorsa condivisa, è un buffer circolare di dimensione data (ad esempio 20) il cui stato è :
        • numero di elemento contenuti: count;
        • puntatore alla prima posizione libera: writepos;
        • puntatore al primo elemento occupato: readpos;
    II produttore, inserisce, 20 numeri interi in maniera sequenziale. 
    Il consumatore li estrae sequenzialmente per stamparli. 
    Il programma dovrà prevedere:
        • un meccanismo di accesso controllato alla risorsa buffer da parte dei due Thread 
          (mutex per il controllo della mutua esclusione nell' accesso al buffer)
        • una sincronizzazione tra il produttore ed il consumatore (Thread) in caso di:
            • buffer pieno: definizione di una condition per la sospensione del produttore
              se il buffer è pieno (notfull);
            • buffer vuoto: definizione di una condition per la sospensione del consumatore 
              se il buffer è vuoto (notempty);
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

int buffer[20];        // Buffer circolare di size 20
int count = 0;        // Numero di element contenuti nel buffer
int *writepos = 0;   // Puntatore alla prima posizione libera
int *readpos = 0;   // Puntatore al primo elemento occupato

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t varCond = PTHREAD_COND_INITIALIZER;


void *routineProduttore()
{   

    for (int i = 0; i < 20; i++)
    {
        pthread_cond_wait(&varCond,&myMutex);
        //Vado in M.E. per inserire i numeri nel buffer
        pthread_mutex_lock(&myMutex);
            buffer[i] = i;
        pthread_mutex_unlock(&myMutex);
        pthread_cond_signal(&varCond);
    }

}

void *routineConsumatore()
{


}

int main(int *argc, char *argv[])
{

    pthread_t thProduttore, thConsumatore;


    //Creazione thread
    if (pthread_create(&thProduttore, NULL, routineProduttore, NULL) != 0)
    {
        printf("\nErrore nella creazione del thread produttore");
        return -1;
    }
    
    if (pthread_create(&thConsumatore, NULL, routineConsumatore, NULL) != 0)
    {
        printf("\nErrore nella creazione del thread consumatore");
        return -1;
    }

    //Fase di JOIN
    if (pthread_join(&thProduttore, NULL) != 0)
    {
        printf("\nErrore nella fase di JOIN del thread produttore");
        return -2;
    }
    
    if (pthread_join(&thConsumatore, NULL) != 0)
    {
        printf("\nErrore nella fase di JOIN del thread consumatore");
        return -2;
    }


    return 0;
}