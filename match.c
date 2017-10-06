#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#define NB_SONG 3

struct thread_params{
    char* song;
    int number_of_performance;
};

typedef struct thread_params thread_params;

void *supporter (void *arg){
  thread_params *params  = (thread_params*) arg;
  int       i ;
  int       pid ;
  pthread_t tid ;  
  pid = getpid () ;
  tid = pthread_self () ;
  srand ((int) tid) ;

  for (i = 0; i < params->number_of_performance; i++){
    printf ("Processus %d Thread %x : %s \n", pid, (unsigned int) tid, 
            params->song);
    usleep (rand() / RAND_MAX * 1000000.) ;
  }
  return (void *) tid ;
}

int main (int argc, char **argv){

  int team1;
  int team2;
  char song1[] = "Allons enfants de la patrie";
  char song2[] = "Swing low, sweet chariot";
  thread_params t1_params, t2_params;
  int i;
  int nb_threads = 0;
  pthread_t *tids;

  if (argc != 5){
    fprintf(stderr, "usage : %s <team1 supporter> <num of time team1 sing>\n"
            "\t<team2 supporter> <num of time team2 sing>\n", argv[0]) ;
    exit (-1) ;
  }

  // Création des structures pour les équipes
  team1 = atoi (argv[1]);
  t1_params.number_of_performance = atoi(argv[2]);
  t1_params.song = (char*) malloc(sizeof(strlen(song1)));
  t1_params.song = song1;

  team2  = atoi (argv[3]);
  t2_params.number_of_performance = atoi(argv[4]);
  t2_params.song = (char*) malloc(sizeof(strlen(song2)));
  t2_params.song = song2;

  nb_threads = team1 + team2; 
  tids = malloc (nb_threads*sizeof(pthread_t)) ;

  /* Create the threads for team1 */
  for (i = 0 ; i < team1; i++){
    pthread_create (&tids[i], NULL, supporter, &t1_params) ;
  }
  /* Create the other threads (ie. team2) */
  for ( ; i < nb_threads; i++){
    pthread_create (&tids[i], NULL, supporter, &t2_params) ;
  }  

  /* Wait until every thread ended */ 
  for (i = 0; i < nb_threads; i++){
    pthread_join (tids[i], NULL) ;
  }
  
  free (tids) ;
  return EXIT_SUCCESS;
}
