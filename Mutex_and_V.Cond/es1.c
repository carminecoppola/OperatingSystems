/*  Si realizzi un programma in C e Posix sotto Linux che, con l'ausilio della libreria Pthread, 
    lancia m thread per calcolare la somma dei prodotti delle colonne di due matrici mxm. 
    L'i-esimo thread, dopo aver calcolato la somma dei prodotti delle colonne i-esime delle due 
    matrici, inserisce il risultato in un array m-dimensionale, in modo concorrente, nella prima
    locazione libera disponibile. Non appena l'array sara riempito, un m+1-esimo thread, che era
    in attesa, provvedera a stampare il contenuto. Le matrici devono essere allocate dinamicamente.
    Usare mutex e variabili di condizione.  */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>



int main(int argc, char* argv)
{


    return 0;
}