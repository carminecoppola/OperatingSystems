/*  Si realizzi un programma in C sotto Linux che preso un intero N come parametro da riga
    di comando (compreso tra 1 e 1000) generi due figli F1 e F2 e proceda come segue:
        • Se il parametro N è pari, il programma invia il segnale SIGUSR1 al figlio F1 
          che calcola la somma tra il valore del suo PID e quello del processo padre. 
          Il processo F2 stamperà solo il suo PID.
        • Viceversa, se il parametro N è dispari, il programma invia il segnale SIGUSR2
          al figlio F2 che calcola la differenza tra il valore del PID del processo padre
          e il suo PID. In questo caso, il processo F1 stamperà solo il suo PID.
        • Alla fine, il processo principale calcolerà e stamperà il prodotto dei PID dei 
          due processi figli F1 e F2 e terminerà stampando un messaggio di saluto. 
*/

#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void somma()
{
  pid_t pidPadre = getpid();
  pid_t pidFiglio = getpid();

  int somma = pidPadre + pidFiglio;

  printf("Figlio 1 (PID: %d): Somma = %d\n", pidFiglio, somma);
  exit(0);

}

void differenza()
{
  pid_t pidPadre = getpid();
  pid_t pidFiglio = getpid();

  int differenza = pidPadre - pidFiglio;

  printf("Figlio 2 (PID: %d): Differenza = %d\n", pidFiglio, differenza);
  exit(0);

}

int main(int argc, char*argv[])
{ 
  
  int N = atoi(argv[1]);

  if (N < 1 || N > 1000) {
      printf("Il parametro N deve essere compreso tra 1 e 1000.\n");
      return 1;
  }
  
  
  pid_t pid_figlio1, pid_figlio2;

  pid_figlio1 = fork();

  if (N % 2 == 0)
  {
    printf("\nIl pid è pari");
    signal(SIGUSR1, somma);
  }
  else
  {
    pid_figlio2 = fork();
    if (pid_figlio2 == 0)
    {
      signal(SIGUSR2, differenza);
    }
    else
    {
      printf("\nIl pid è dispari");
      kill(pid_figlio2,SIGUSR2);
    }

  }

  return 0;
}