/*  Un processo padre crea M (con M numero pari) processi figli. Ciascun processo figlio Fi è identificato
    da una variabile intera i (i=0,1,2,3, ..., M-1). Si gestiscano i seguenti due casi:
        1. Se il primo argomento dato in input al programma (ovvero argv[]) è uguale al carattere 'p'
           (che sta per "pari") ogni processo figlio Fi con i pari (ovvero se M=6, FO, F2, F4) manda 
           un segnale (SIGUSR1) al processo i+ 1 (ovvero a FI, F3, F5 nel caso in cui M=6);

        2. Se invece arg[1] è uguale al carattere 'm' (che sta per "metà") ogni processo figlio Fi con
           I < M/2 (ovvero se M=6, FO, FI, F2) manda un segnale (SIGUSRI) all'altra metà del processi,
           ovvero al processo i + M/2 (ovvero a F3, F4, F5 nel caso in cui Me6); */