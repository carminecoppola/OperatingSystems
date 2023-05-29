/* 
   Scrivere un programma che prende in input un intero n, il nome di un file di testo ed un carattere x
   • Il programma ha il compito di contare le occorrenze del carattere x nel file di testo
   • Il programma esegue tale compito creando n thread, ognuno dei quali esamina una porzione diversa del file di testo
   • ad esempio, se il file è lungo 1000 bytes ed n=4, il primo thread esaminerà i primi 250 bytes, il secondo thread esaminerà i 250 bytes successivi, e così via
   (Ho già scritto un file di esempio "file1.txt")
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

char carattere;  // Carattere da cercare nel file

typedef struct parameter
{
    FILE *file;  // Puntatore al file di testo
    int bytes;   // Numero di byte da esaminare per il thread
    int p_file;  // Posizione di partenza nel file
    int occorrenza;  // Contatore delle occorrenze del carattere
} parameters;


//Funzione eseguita dai thread per contare le occorrenze del carattere specificato in una porzione di file.
void *routine(void *args)
{
    parameters *data = (parameters *)args; // Cast del puntatore args alla struttura parameters

    int occorrenza = 0;

    fseek(data->file, data->p_file, SEEK_SET);  // Sposta il puntatore della posizione del file alla posizione di partenza del thread

    // Andiamo a leggere i caratteri nella porzione di file assegnata al thread
    for (int i = 0; i < data->bytes; i++)
    {
        int f = fgetc(data->file);  // Legge un carattere dal file

        if ((char)f == carattere)  // Confronta il carattere letto con quello da cercare
        {
            occorrenza++;  // Incrementa il contatore delle occorrenze se i caratteri corrispondono
        }
    }

    data->occorrenza = occorrenza;  // Salva il numero di occorrenze nel parametro del thread
    pthread_exit(NULL);
}


int main()
{
    int n;  // Numero di thread
    char filename[100];  // Nome del file di input

    printf("\nInserisci N: ");
    scanf("%d", &n);
    printf("Inserisci il nome del file di input: ");
    scanf("%s", filename);
    printf("Inserisci il carattere da ricercare: ");
    scanf(" %c", &carattere);

    FILE *file = fopen(filename, "r");  // Apre il file in modalità lettura

    //Se il file non esiste ritorniamo un errore
    if (file == NULL)
    {
        printf("Impossibile aprire il file.\n");
        return 1;
    }

    int num_bytes;

    /*La fseek() posiziona il puntatore di posizione alla fine, questo è fatto per ottenere
      la posizione corrente del puntatore di posizione, che corrisponde alla dimensione totale del 
      file in byte. Questo valore viene successivamente memorizzato nella variabile num_bytes.
    */
    fseek(file, 0, SEEK_END);
    num_bytes = ftell(file);   // Ottiene la posizione corrente del puntatore di posizione del file (la dimensione del file in byte)
   
   //Controllo che il numero di bytes sia divisibile per n
    if (num_bytes % n != 0)
        num_bytes = (num_bytes / n + num_bytes % n);
    else
        num_bytes = num_bytes / n;

    pthread_t *threads = malloc(n * sizeof(pthread_t));  // Alloca memoria per gli identificatori dei thread

    if (threads == NULL)
    {
        printf("Errore nell'allocazione della memoria per i thread.\n");
        fclose(file);
        return 1;
    }

    parameters *data = malloc(n * sizeof(parameters));  // Alloca memoria per i parametri dei thread

    if (data == NULL)
    {
        printf("Errore nell'allocazione della memoria per i parametri.\n");
        fclose(file);
        free(threads);
        return 1;
    }

    int cont_p_file = 0;

    for (int i = 0; i < n; i++)
    {
        data[i].bytes = num_bytes;
        data[i].file = file;
        data[i].p_file = cont_p_file;
        data[i].occorrenza = 0;

        if (pthread_create(&threads[i], NULL, routine, (void *)&data[i]) != 0)  // Crea un thread passando i parametri
        {
            printf("Errore nella creazione dei thread.\n");
            fclose(file);
            free(threads);
            free(data);
            return 1;
        }

        cont_p_file += num_bytes;
    }

    int count = 0;

    for (int i = 0; i < n; i++)
    {
        pthread_join(threads[i], NULL);  // Attendere la terminazione dei thread
        count += data[i].occorrenza;  // Sommare il numero di occorrenze restituito da ciascun thread
    }

    fclose(file);
    free(threads);
    free(data);

    printf("\nIl carattere '%c' si ripete %d volte nel file.\n", carattere, count);

    return 0;
}