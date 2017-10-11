#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 20

// TODO: arrêter les autres threads quand on a trouver l'élément

/* Struct: search_param
 * tab: adresse du tableau d'élément
 * length: l'indice de fin du tableau à parcourir
 * start_index: l'indice à partir duquel commencer la recherche
 * elt: l'élément recherché
 * ret: l'index ou a été trouvé l'élément
 */
struct search_param{
    int* tab;
    int length;
    int start_index;
    int elt;//élément recherché
    int ret;//-1 si l'élément n'est pas trouvé sinon indice de l'élément trouvé
};

/* global flag to tell that search is not over (0) */
int search_status = 0;

typedef struct search_param search_param;

/* Fonction: search
 * On recherche un élément dans un tableau donné dans une structure.
 *
 * params: la structure contenant les informations importantes
 *         (plus d'infos disponible en commentaire au dessus de la
 *         struct search_param).
 *
 *  return : l'index ou a été trouvé l'élément, -1 sinon.
 */
void* search (void* struct_params){
    search_param* params = (search_param*) struct_params;
    printf("Thread lancé...\n");
    printf("DBG: length: %d start_index: %d elt: %d\n",
            params->length, params->start_index, params->elt);
    int index = params->start_index;
    for (; index < (params->length) && !search_status; index++){
        if((params->tab[index]) == (params->elt)){
            params->ret = index;
            search_status = 1;
            return NULL;
        }
    }
    return NULL;
}

/* Fonction: usage
 * Affiche les paramètres que prend le programme.
 * 
 * program: le nom du programme actuel
 */
void usage(char* program){
    printf("Usage: %s <element being searched> <file with array>"
            " <nb of threads>\n", program);
    exit(0);
}

/* Fonction: load_vector
 * On charge le contenu du fichier donné en paramètre.
 * On compte tout d'abord le nombre de lignes pour créer un tableau
 * suffisament grand, puis on charge le contenu du fichier
 * dans ce tableau.
 *
 * filename : le nom du fichier à ouvrir
 * vector : l'endroit ou stocker le tableau d'int
 * size : l'endroit ou stocker la taille du tableau généré
 *
 * return : un pointeur sur le tableau d'entier récupéré
 */
void load_vector(char* filename, int** vector, int* size){
    FILE* fd = NULL;
    char buf[BUFFER_SIZE];
    int element_count = 0;
    int array_iterator = 0;
    if ((fd = fopen(filename, "r")) == NULL){
        perror("opening file");
        exit(1);
    }
    //compter les lignes
    while (fgets(buf, BUFFER_SIZE, fd) != NULL)
        element_count++;

    if (element_count == 0){
        printf("Error: file is empty.\n");
        exit(1);
    }

    *vector = malloc(sizeof(int) * element_count);

    rewind(fd);

    //remplir tableau
    while (fgets(buf, BUFFER_SIZE, fd) != NULL){
        (*vector)[array_iterator] = atoi(buf);
        array_iterator++;
    }

    *size = array_iterator;
}

/* Fonction: print_result
 * Affiche le résultat de la recherche.
 *
 * params: adresse de la structure contenant les résultats.
 */
void print_result(int size, search_param params[size]){
    int index_found = -1;
    for (int i = 0; i < size; i++){
        if (params[i].ret != -1){
            index_found = i;
            break;
        }
    }
    if (index_found == -1){
        printf("L'élément %d n'a pas été trouvé.\n", params[0].elt);
    } else {
        printf("L'élément %d a été trouvé à l'indice %d.\n", 
                params[index_found].elt,
                params[index_found].ret);
    }
}

int main (int argc, char **argv){
    if (argc < 4){
        usage(argv[0]);
    }
    // (paramètres:elt,fichier,nbthreads)
    int eltRecherche = atoi(argv[1]);
    // creer tableau à partir du deuxième argument
    int* tableau = NULL;
    int size_tab = 0;
    int nb_threads = atoi(argv[3]);
    int tailleParThread = 0;
    int array_elt_index = -1;

    pthread_t threads[nb_threads];
    search_param thread_structs[nb_threads];

    load_vector(argv[2], &tableau, &size_tab);

    tailleParThread = size_tab/nb_threads;

    /* initialisation des informations */
    for (int i = 0; i < nb_threads; i++){
        thread_structs[i].elt = eltRecherche;
        thread_structs[i].ret = array_elt_index;
        thread_structs[i].start_index = i*tailleParThread;
        thread_structs[i].tab = tableau;
        thread_structs[i].length = (i + 1)*tailleParThread;
    }

    /* Le dernier thread parcourt la longueur de base + le reste
     * éventuel du tableau
     */
    thread_structs[nb_threads - 1].length += size_tab % nb_threads;

    /* initialisation des différents threads */
    for (int i = 0; i < nb_threads; i++){
        pthread_create(&threads[i], NULL, search, &thread_structs[i]);
    }
    
    /*joint des differents threads*/
    for (int i = 0; i < nb_threads; i++){
        pthread_join(threads[i], NULL);
    }

    print_result(nb_threads, thread_structs);

    return EXIT_SUCCESS;
}
