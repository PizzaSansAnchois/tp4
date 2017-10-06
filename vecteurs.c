#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

struct search_param{
  char* tab;
  int length;
  int start_index;
  char elt;
  int ret;
};

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

int main (int argc, char **argv){


  return EXIT_SUCCESS;
}
