#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

struct search_param{
  int* tab;
  int length;
  int start_index;
  int elt;
  int ret;
};

/* TODO: changer le nom (structure est super vague */
typedef struct search_param structure;

void *search (void *arg){
  structure* params=(structure*)arg;
  int index=(params->start_index);
  while(index!=(params->length)){
    if((params->tab[index])==(params->elt)){
      params->ret=index;
      return params;
    }
    index++;
  }
  params->ret=-1;
  return params;
}

void usage(char* program){
    printf("Usage: %s <element being searched> <file with array>" 
            " <nb of threads>\n", program);
    exit(0);
}

int main (int argc, char **argv){
    if (argc < 4){
        usage(argv[0]);
    }
  //(paramètres=elt,fichier,nbthreads)
  int eltRecherche=atoi(argv[1]);
  int* tableau=NULL;//creer tableau à partir du deuxième argument
  int nbThreads=atoi(argv[3]);

  pthread_t *tids = malloc (nbThreads*sizeof(pthread_t));
  //tableau de structures pour passer les arguments
  structure *paramThreads = malloc (nbThreads*sizeof(structure));

  // Création des structures
  structure tempStruct;

  //valeurs communes entre threads
  tempStruct.elt=eltRecherche;
  tempStruct.ret=-1;
  tempStruct.tab=tableau;
  int tailleParThread=sizeof(tableau)/nbThreads;
  tempStruct.length=tailleParThread;

  //valeurs variables des threads
  int i;
  // TODO: ERROR ICI
  for (i = 0 ; i < nbThreads-1; i++){
    tempStruct.start_index=tailleParThread*i;
    paramThreads[i]=tempStruct;
  }
  //création de la structure du dernier thread (avec le reste des int)
  tempStruct.length=tailleParThread+(sizeof(tableau)%nbThreads);
  tempStruct.start_index=tailleParThread*(nbThreads-1);
  paramThreads[nbThreads-1]=tempStruct;

  /* Create the threads for search*/
  for (i = 0 ; i < nbThreads-1; i++){
    pthread_create (&tids[i], NULL, search, &(paramThreads[i])) ;
  }

  /* Wait until every thread ended */ 
  for (i = 0; i < nbThreads; i++){
    pthread_join (tids[i], NULL) ;
  }
  
  free (tids) ;
  free (paramThreads);

  return EXIT_SUCCESS;
}
