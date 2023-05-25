/* Scrivere un programma che prende in input un intero n, il nome di un file di testo ed un carattere x
    • Il programma ha il compito di contare le occorrenze del carattere x nel file di testo
    • Il programma esegue tale compito creando n thread, ognuno dei quali esamina una porzione diversa del file di testo
    • ad esempio, se il file è lungo 1000 bytes ed n=4, il primo thread esaminerà i primi 250 bytes, il secondo thread esaminerà i 250 bytes successivi, e così via
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

char carattere;

typedef struct parameter
{
    FILE *file;
    int bytes;
    int p_file;
    int occorrenza;

}parameters;


void *routine(void *args)
{

    parameters *data = (parameters *)args;
    
    printf("\nBytes: %d", data->bytes);
    printf("\nP_file: %d", data->p_file);

    int occorrenza = 0;

    fseek(data->file, data->p_file, SEEK_SET);

    for (int i = 0; i < data->bytes; i++)
    {
        int f = fgetc(data->file);
        printf("\n Carattere: %c", (char)f);

        if ((char)f == carattere)
        {
            occorrenza++;
        }
    }
    data->occorrenza = occorrenza;
    pthread_exit(NULL);
}


int main()
{
    int n;
    char filename[100];

    printf("\nInserisci N:");
    scanf(" %d", &n);
    printf("\nInserisci il nome del file di input:");
    scanf(" %s", filename);
    printf("\nInserisci il carattere da ricercare:");
    scanf(" %c", &carattere);
    
    FILE *file;

    file = fopen(filename,"r");

    int num_bytes;

    // Sposta il puntatore di posizione del file alla fine
    fseek(file, 0, SEEK_END);

    // Ottieni la posizione corrente del puntatore di posizione del file
    num_bytes = ftell(file);

    printf("NUM b= %d",num_bytes);
    printf("\nNUM bytes singolo thread= %d",num_bytes/n);

    pthread_t* threads = (pthread_t*)malloc(n * sizeof(pthread_t));

    if (threads == NULL) {
        printf("Errore nell'allocazione della memoria per i thread\n");
        return 1;
    }

    parameters data[n];

    int cont_p_file = 0;

    for (int i = 0 ; i < n; i++)
    {
        data[i].bytes = num_bytes/n;
        data[i].file = file;

        cont_p_file += num_bytes/n;

        data[i].p_file = cont_p_file;
        data[i].occorrenza = 0;
        
        if (pthread_create(&threads[i], NULL, routine, (void*)&data[i]) != 0) {
            printf("Errore nella creazione dei thread\n");
            return 1;
        }

    }
    
    int count = 0;

    for (int i = 0; i < n; i++)
    {
        pthread_join(threads[i], NULL);
        count += data[i].occorrenza;
    }

    fclose(file);

    printf("\nOccorrenze totali: %d\n", count);

    return 0;
}
