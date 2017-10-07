#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 20

/* Struct: search_param
 * tab: adresse du tableau d'élément
 * length: longueur du tableau
 * start_index: l'indice à partir duquel commencer la recherche
 * elt: l'élément recherché
 * ret: l'index ou a été trouvé l'élément
 */
struct search_param{
    int* tab;
    int length;
    int start_index;
    int elt;
    int ret;
};

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
void search (search_param* params){
    int index = params->start_index;
    for (; index < (params->length); index++){
        if((params->tab[index]) == (params->elt)){
            params->ret = index;
            return;
        }
    }
}

/* Fonction: usage
 * Affiche les paramètres que prend le programme.
 * 
 * program: le nom du programme actuel
 */
void usage(char* program){
    printf("Usage: %s <element being searched> <file with array>\n", program);
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
    while (fgets(buf, BUFFER_SIZE, fd) != NULL)
        element_count++;

    if (element_count == 0){
        printf("Error: file is empty.\n");
        exit(1);
    }

    *vector = malloc(sizeof(int) * element_count);

    rewind(fd);

    while (fgets(buf, BUFFER_SIZE, fd) != NULL){
        (*vector)[array_iterator] = atoi(buf);
        array_iterator++;
    }

    *size = array_iterator;

    /*
    for (int i = 0; i < array_iterator; i++){
        printf("%d : %d\n", i, input_vector[i]);
    }
    */
}

/* Fonction: print_result
 * Affiche le résultat de la recherche.
 *
 * params: adresse de la structure contenant les résultats.
 */
void print_result(search_param* params){
    if (params->ret == -1){
        printf("L'élément %d n'a pas été trouvé.\n", params->elt);
    } else {
        printf("L'élément %d a été trouvé à l'indice %d.\n", 
                params->elt,
                params->ret);
    }
}

int main (int argc, char **argv){
    if (argc < 3){
        usage(argv[0]);
    }
    // (paramètres=elt,fichier,nbthreads)
    int eltRecherche = atoi(argv[1]);
    // creer tableau à partir du deuxième argument
    int* tableau = NULL;
    int size_tab = 0;
    int nbThreads = 1;
    int tailleParThread = 0;
    int array_elt_index = -1;

    load_vector(argv[2], &tableau, &size_tab);

    tailleParThread = size_tab/nbThreads;

    // Création des search_params
    search_param tempStruct;

    tempStruct.elt = eltRecherche;
    tempStruct.ret = array_elt_index;
    tempStruct.tab = tableau;
    tempStruct.length = tailleParThread;

    search(&tempStruct);

    print_result(&tempStruct);

    return EXIT_SUCCESS;
}
