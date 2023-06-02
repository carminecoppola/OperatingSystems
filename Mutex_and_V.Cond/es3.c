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

