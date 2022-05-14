#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "../inc/k-arbre.h"
#include "../inc/alloc.h"

void kAfficherRec(karbre A, int prof);

karbre kArbreVide(){
  return NULL;
}

karbre kConsArbre(int k, element racine, ...){
  karbre ka = safe_alloc(sizeof(struct karbre));

  ka->k = k;
  ka->fils = safe_alloc(sizeof(element) * k);

  ka->racine = racine;

  va_list list;

  va_start(list, racine);

  for (int i = 0; i < k; i++){
    ka->fils[i] = va_arg(list, karbre);
  }
  
  va_end(list);

  return ka;
}

karbre kFils(int ieme, karbre A){
  if (kEstVide(A) || ieme < 0 || ieme >= A->k){ return NULL; }
  return A->fils[ieme];
}

element kRacine(karbre A){
  if (kEstVide(A)) { return NULL; }
  return A->racine;
}

int kEstVide(karbre A){
  return (A == NULL);
}

void kAfficher(karbre A){
  kAfficherRec(A, 0);
}

void kAfficherRec(karbre A, int prof){
  if (kEstVide(A)) { return; }
  
  for (int i = 0; i < prof; i++){ printf("  "); }
  
  printf("> Racine: %p (prof. = %d)\n", A->racine, prof);

  for (int i = 0; i < A->k; i++){ kAfficherRec(A->fils[i], prof + 1); }
  
}