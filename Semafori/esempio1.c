/*  Implementazione del Produttore - Consumatore con i semafori.
    I passi che si possono verificare in esecuzione sono:
        1.  Il Produttore inizializza il buffer e i due semafori;

        2.  Assumiamo, poi, che venga eseguito il Consumatore, 
            esso si blocca nella sua chiamata a sem_wait() poiché 
            il valore di get è 0;

        3.  Dopo qualche istante, inizia il Produttore, quando 
            chiama sem_wait(), il valore di put è decrementato da
            1 a 0, e il Produttore pone un elemento nel buffer;

                •   Poi invoca una sem_post() per incrementare il valore di get da 0 a 1.
                    Poiché è presente un thread bloccato (in attesa che il valore diventi
                    positivo) sul semaforo get;
                
                •   Assumiamo che il Produttore continui la sua esecuzione, allora il 
                    Produttore si blocca nella sua chiamata a sem_wait() in cima al ciclo
                    for, perché il valore di put è 0. Il Produttore deve attendere fino a 
                    che il Consumatore svuoti il buffer;
        
        4.  Il Consumatore ritorna dalla sua chiamata a sem_wait(), il che decrementa il 
            valore del semaforo get da 1 a 0. Elabora l’elemento nel buffer e chiama 
            sem_post() che incrementa il valore di put ad 0 a 1. Poiché un thread è
            bloccato su questo semaforo (il produttore), in attesa che il valore sia > 0,
            il thread viene marcato come pronto per l’esecuzione. 
            Ma assumiamo che il consumatore continui l’esecuzione. 
            Il consumatore si blocca nella sua chiamata a sem_wait(), in cima al ciclo for,
            poiché il valore di get è 0;
        
        5.  Il produttore ritorna dalla sua chiamata a sem_wait(), pone un elemento nel 
            buffer e lo schema si ripete;
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

#define FILE_MODE S_IRUSR | S_IWUSR

int main(int argc, char* argv[])
{
    int c, flags;
    sem_t* sem_cs;
    unsigned int value = 1;
    flags = O_CREAT;
    int optind = argc - 1;
    char* optarg;

    // Analisi delle opzioni della riga di comando utilizzando la funzione getopt

    /*  La funzione getopt restituisce -1 quando non ci sono più opzioni da analizzare.
        All'interno del ciclo switch, vengono gestite le opzioni -e e -i. 
        L'opzione -e imposta il flag O_EXCL per indicare che il semaforo deve essere 
        creato in esclusiva. L'opzione -i imposta il valore iniziale del semaforo 
        utilizzando atoi per convertire l'argomento in un intero.   */

    while ((c = getopt(argc, argv, "Ciao:")) != -1) {
        switch (c) {
            case 'e':
                flags |= O_EXCL;  // Imposta il flag O_EXCL per la creazione esclusiva del semaforo
                break;
            case 'i':
                value = atoi(optarg);  // Imposta il valore iniziale del semaforo
                break;
        }
    }

    // Controllo se l'argomento finale è stato specificato correttamente
    if (optind != argc - 1) {
        fprintf(stderr, "usage: %s [-e] [-i initialvalue] <name>", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Apertura del semaforo con nome
    sem_cs = sem_open(argv[optind], flags, FILE_MODE, value);

    // Chiusura del semaforo
    sem_close(sem_cs);

    // Rimozione del semaforo
    sem_unlink(argv[optind]);

    exit(EXIT_SUCCESS);

    return 0;
}
