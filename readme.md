# Autograms

### Algoritmo

Viene proposto un algoritmo di minimizzazione per la risoluzione degli autogrammi.

Passi dell'algoritmo:
* Si parte da una soluzione casuale.
* Si raffina la soluzione iterativamente.
* Se il processo non converge, si torna al passo iniziale.

L'algoritmo termina quando:
* Un processo di minimizzazione converge al risultato corretto.
* Viene raggiunto il numero massimo di iterazioni specificato.

Classi principali:
* `autogram_solver       ` ~ Implementazione dell'algoritmo descritto.
* `autogram_solver_helper` ~ Esecuzione parallela di più istanze dell'algoritmo.

### Utilizzo

Per avviare il programma da linea di comando si deve utilizzare la seguente sintassi:
* `autograms.exe` `sentence` `-i max iterations` `-o force-pangram`

Parametri:
* `sentence         ` ~ Frase da analizzare.
* `-i max iterations` ~ Aumenta il numero massimo di iterazioni (opzionale, default 1000000).
* `-o force-pangram ` ~ Forza la ricerca di un autogramma che sia anche un pangramma (opzionale).

Esempi di utilizzo:
* `autograms.exe` `"this sentence contains"`
* `autograms.exe` `"this sentence contains"` `-i 2000000`
* `autograms.exe` `"this sentence contains"` `-o force-pangram`
* `autograms.exe` `"this sentence contains"` `-i 2000000` `-o force-pangram`

### Alcuni autogrammi

* `autograms.exe` `"this sentence contains"`
    * this sentence contains three a's, three c's, two d's, twenty-seven e's, two f's, two g's, seven h's, eight i's, one l, one m, nineteen n's, ten o's, six r's, twenty-six s's, twenty-two t's, two u's, three v's, nine w's, three x's and four y's

* `autograms.exe` `"only the fool would take trouble to verify that his sentence was composed of"`
    * only the fool would take trouble to verify that his sentence was composed of five a's, two b's, three c's, four d's, thirty-four e's, eight f's, four g's, ten h's, eleven i's, two k's, seven l's, two m's, seventeen n's, twenty o's, two p's, one q, nine r's, thirty-two s's, twenty-eight t's, six u's, eight v's, eleven w's, two x's and seven y's

### Alcuni autogrammi che sono anche pangrammi

* `autograms.exe` `"this sentence contains"` `-o force-pangram`
    * this sentence contains three a's, one b, three c's, two d's, thirty-five e's, six f's, two g's, seven h's, eleven i's, one j, one k, two l's, one m, twenty-two n's, fifteen o's, one p, one q, five r's, twenty-six s's, twenty-one t's, one u, seven v's, eight w's, three x's, five y's and one z

* `autograms.exe` `"only the fool would take trouble to verify that his sentence was composed of"` `-o force-pangram`
    * only the fool would take trouble to verify that his sentence was composed of five a's, two b's, three c's, four d's, thirty-seven e's, eight f's, four g's, eleven h's, thirteen i's, one j, two k's, seven l's, two m's, twenty-one n's, twenty-two o's, two p's, one q, nine r's, twenty-nine s's, thirty t's, five u's, eight v's, twelve w's, one x, eight y's and one z

* `autograms.exe` `"this computer generated pangram contains"` `-o force-pangram`
    * this computer generated pangram contains six a's, one b, three c's, three d's, thirty-seven e's, six f's, three g's, nine h's, twelve i's, one j, one k, two l's, three m's, twenty-two n's, thirteen o's, three p's, one q, fourteen r's, twenty-nine s's, twenty-four t's, five u's, six v's, seven w's, four x's, five y's and one z
