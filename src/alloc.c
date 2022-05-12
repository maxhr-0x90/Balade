#include <stdio.h>
#include "../inc/alloc.h"

void *safe_alloc(int taille){
  void *espace = malloc(taille);

  if (espace == NULL) {
    fprintf(stderr, "Err: echec de l'allocation\nFin du programme");
    exit(-1);
  }

  return espace;
}

void *safe_realloc(void *ptr, int taille){
  void *espace = realloc(ptr, taille);

  if (espace == NULL) {
    fprintf(stderr, "Err: echec de la réallocation\nFin du programme");
    exit(-1);
  }

  return espace;
}

void safe_free(void *ptr){
  if (ptr != NULL) {
    free(ptr);
  }
}