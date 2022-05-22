/* Moddule d'alloction mémoire sécurisé */

#ifndef ALLOC_H_
#define ALLOC_H_

#include <stdlib.h>

// Allocation de taille espace
void *safe_alloc(int taille);
// Libération de l'espace occupé
void safe_free(void *ptr);
// Réallocation de l'espace mémoire
void *safe_realloc(void *ptr, int taille);

#endif