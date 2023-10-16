# Autograms

### Algoritmo

Per la risoluzione degli autogrammi viene proposto un algoritmo di minimizzazione.

Passi dell'algoritmo:
* Si parte da una soluzione casuale.
* Si raffina la soluzione iterativamente.
* Se il processo non converge, si torna al passo iniziale.

L'algoritmo termina quando:
* Un processo di minimizzazione converge al risultato corretto.
* Viene raggiunto il numero massimo di iterazioni specificato.

Classi principali:
* `autogram_solver` : Implementa l'algoritmo descritto.
* `autogram_solver_helper` : Esegue parallelamente più istanze dell'algoritmo.

### Utilizzo

Per avviare il programma da linea di comando si deve utilizzare la seguente sintassi:
* `autograms.exe` `sentence` `-i max iterations` `-o force-pangram`

Parametri:
* `sentence` : Frase da analizzare.
* `-i max iterations` : Aumenta il numero massimo di iterazioni (default 500000).
* `-o force-pangram` : Forza la ricerca di un autogramma che sia anche un pangramma.

Esempi di utilizzo:
* `autograms.exe` `"this sentence contains"`
* `autograms.exe` `"this sentence contains"` `-i 1000000`
* `autograms.exe` `"this sentence contains"` `-o force-pangram`
* `autograms.exe` `"this sentence contains"` `-i 1000000` `-o force-pangram`
