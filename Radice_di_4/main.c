#include <stdio.h>
#include"funzione.h"

int funzione(int);

int main(int argc, char *argv[])
{
    int x,s;

    printf("Inserisci un intero: ");
    scanf("%d",&x);

    s=funzione(x);
    if(s!=-1)
        printf("La radice quadrata di %d: %d\n",x,s);
}