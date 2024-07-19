Per tutte le cartelle Esercizio_02 valgono le seguenti istruzioni per:

	compilare: make

	eseguire:  make run

	cancellare i file prodotti: make clean

Nelle cartelle Esercizio_02.2 sono presenti due Makefile, che compilano ed eseguono dei file diversi.
Il programma Make, per le sue impostazioni, cerca spontaneamente nella cartella di lavoro 
il file di nome Makefile, quindi riconosce come Makefile solo il file di nome Makefile. 
Per evitare confusione, è buona norma mantenere attivo un solo Makefile nella cartella di lavoro.

Per come ho impostato Makefile, compila ed esegue i codici media_blocchi e main.
Se si vuole compilare ed eseguire il codice passi_reticolo, il suo Makefile è pronto. 
Per usarlo bisogna rinominare Makefile in un altro modo e rinominare Makefile_passi 
come Makefile. In questo modo, digitando i comandi sopra, si esegue anche l'altro codice. 

