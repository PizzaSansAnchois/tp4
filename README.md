# README et compte-rendu du tp4

Le programme file_gen.c permet de générer une suite de nombres qui seront
écrites dans un fichier.

Utilisation : ./file_gen <number of number> <seed>

Il est possible de choisir le nombre de lignes générées dans le fichiers,
ainsi que la graine.
Un fichier contenant 100 000 000 lignes est suffisament gros pour tester
l'efficacité des programmes threadés.

Le paragraphe 2 est traité dans le fichier match.c

La question 6 est le fichier seq_vector.c.
Usage: ./seq_vector <element being searched> <file with array>

La question 7 est le fichier thd_vector.c.
Usage: ./thd_vector <element being searched> <file with array> <nb of threads>

Questions
1. tids est un tableau qui contient les structures des threads du programme.
On utilise la fonction malloc pour alloué un tableau de taille suffisante, 
et pthread_create est utilisée pour initialisé chacun des pthread_t contenus.
L'espace mémoire alloué à cette variable est libéré au moment du free(tids).

2. pthread_create permet de créer un thread, tant qu'on lui fourni l'adresse
du pthread_t que l'on souhaite créer, des options de threads, la fonction
que va exécuté le thread, ainsi que l'argument que l'on passera à cette
fonction. Les arguments et options sont facultatifs.

3. usleep est l'équivalent de sleep, sauf qu'au lieu de "dormir" pendant des 
secondes, le thread "dort" pendant des  microsecondes. C'est dire que le
thread laisse le CPU pour un autre thread pendant au moins l'intervalle de
temps donné dans la fonction.

4. 
