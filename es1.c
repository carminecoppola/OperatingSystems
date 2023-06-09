/* ESAME: Prova Scritta 20/06/2022
    Scrivere un programma C e Posix che prende da riga di comando N interi
    compresi tra 5 e 20. Il valore di N è costante ed indicato nel sorgente.
    Il programma avvia N thread che hanno il seguente comportamento:

        a)  Ogni thread va in sospensione (invocando la funzione sleep()) per un numero
            di secondi pari al valore del corrispondente parametro e poi stampa il suo indice;

        b)  Alla fine del periodo di attesa (sleep), ogni thread stampa un messaggio: "Thread #: svegliato!";

        c)  Tutti i thread si sincronizzano tra di loro (utilizzando i semafori basati su memoria).

        d)  Dopo aver atteso il termine di tutti gli altri thread, ciascun thread scrive sullo schermo
            che ha terminato ("Thread #: terminato...").
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int N; // Numero di thread da avviare
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER; // Mutex per sincronizzazione
int threadCompletati; // Contatore dei thread completati

void* routine(void* arg)
{
    int indice = (int)arg; // Indice del thread

    pthread_mutex_lock(&myMutex); // Blocca il mutex per leggere il valore di N
    int t = N; // Salva il valore di N nella variabile t
    pthread_mutex_unlock(&myMutex); // Sblocca il mutex

    printf("\n%d-esimo thread sta dormendo...", indice); // Stampa il messaggio di sospensione
    sleep(t); // Mette il thread in pausa per t secondi
    printf("\nThread #%d si è svegliato", indice); // Stampa il messaggio di sveglia

    pthread_mutex_lock(&myMutex); // Blocca il mutex per incrementare il contatore
    threadCompletati++; // Incrementa il contatore dei thread completati
    pthread_mutex_unlock(&myMutex); // Sblocca il mutex

    pthread_exit(NULL); // Termina il thread
}

int main(int argc, char* argv[])
{
    N = atoi(argv[1]); // Legge il valore di N dalla riga di comando

    if (N < 5 || N > 20) // Verifica che N sia compreso tra 5 e 20
    {
        printf("\nErrore: N deve essere compreso tra 5 e 20\n");
        exit(0);
    }
    
    pthread_t th[N]; // Array di thread

    for (int i = 0; i < N; i++)
    {
        if (pthread_create(&th[i], NULL, routine, (void*)i) != 0) // Avvia i thread
        {
            printf("\nErrore nella creazione dei thread\n");
            return -1;
        }
    }

    if (threadCompletati == N) // Controlla se tutti i thread sono completati
    {
        for (int i = 0; i < N; i++)
        {
            printf("\nThread #%d ha terminato.", i); // Stampa il messaggio di completamento per ogni thread
        }
    }

    // Fase di JOIN
    for (int i = 0; i < N; i++)
    {
        if (pthread_join(th[i], NULL) != 0) // Attende il termine dei thread
        {
            printf("\nErrore nella fase di JOIN dei thread\n");
            return -2;
        }
    }
    
    printf("\n"); // Aggiunge una riga vuota per la formattazione

    return 0; // Termina il programma
}
