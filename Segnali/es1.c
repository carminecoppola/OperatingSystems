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
  pid_t pidPadre = getppid();
  pid_t pidFiglio = getpid();

  printf("\n(Somma)Il pid del processo Padre %d, mentre quello del figlio è %d\n", pidPadre, pidFiglio);

  int somma = pidPadre + pidFiglio;

  printf("\nFiglio 1 (PID: %d): Somma = %d\n", pidFiglio, somma);
  exit(0);
}

void differenza()
{
  pid_t pidPadre = getppid();
  pid_t pidFiglio = getpid();

  printf("\n(Differenza)Il pid del processo Padre %d, mentre quello del figlio è %d\n", pidPadre, pidFiglio);

  int differenza = pidPadre - pidFiglio;

  printf("Figlio 2 (PID: %d): Differenza = %d\n", pidFiglio, differenza);
  exit(0);
}



int main(int argc, char* argv[])
{ 
  // Controllo se è stato fornito il parametro N
  if (argc < 2) {
      printf("Parametro N mancante.\n");
      return 1;
  }

  int N = atoi(argv[1]);

  // Controllo se il parametro N è valido
  if (N < 1 || N > 1000) {
      printf("Il parametro N deve essere compreso tra 1 e 1000.\n");
      return 1;
  }
  
  pid_t pid_figlio1, pid_figlio2;

  pid_figlio1 = fork();


  /*  Questa parte viene eseguita solo nel processo figlio 1. 
      Se il parametro N è pari, viene chiamata la funzione somma(). */

  if (pid_figlio1 == 0) {
    if (N % 2 == 0) {
      somma();
    }
  }
  else {
    pid_figlio2 = fork();

    /*  Questa parte viene eseguita solo nel processo figlio 2. 
        Se il parametro N è dispari, viene chiamata la funzione differenza(). */

    if (pid_figlio2 == 0) {
      if (N % 2 != 0) {
        differenza();
      }
    }
    else {
      // Processo padre
      if (N % 2 == 0) {

        /* Nel processo padre, se il parametro N è pari, viene inviato il segnale SIGUSR1
           al figlio 1 utilizzando la funzione kill(). Questo segnale serve come indicazione
           per il figlio 1 di eseguire la funzione somma(). */

        kill(pid_figlio1, SIGUSR1);
      }
      else {

        /* Nel processo padre, se il parametro N è dispari, viene inviato il segnale SIGUSR2
           al figlio 2 utilizzando la funzione kill(). Questo segnale serve come indicazione 
           per il figlio 2 di eseguire la funzione differenza(). */

        kill(pid_figlio2, SIGUSR2);
      }

      int status;

      /* Queste chiamate waitpid() consentono al processo padre di attendere che i suoi figli 
         terminino l'esecuzione. La funzione waitpid() prende il PID del figlio da attendere 
         come primo argomento. Il secondo argomento, &status, è un puntatore a una variabile
         intera in cui viene memorizzato lo stato di uscita del figlio. L'ultimo argomento, 0,
         indica che la chiamata waitpid() è bloccante e il processo padre attende effettivamente
         che i figli terminino. */

      waitpid(pid_figlio1, &status, 0);
      waitpid(pid_figlio2, &status, 0);

      pid_t pidPrincipale = getpid(); // Viene ottenuto il PID del processo padre.
      long prodotto = (long)pid_figlio1 * (long)pid_figlio2; // Calcoliamo il prodotto dei PID dei due figli

      printf("\nIl prodotto dei PID dei due figli è: %ld\n", prodotto);
      printf("\nSaluti dal processo principale (PID: %d)\n", pidPrincipale);
    }
  }

  return 0;
}
