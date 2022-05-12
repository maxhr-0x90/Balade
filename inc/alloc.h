#ifndef ALLOC_H_
#define ALLOC_H_

#include <stdlib.h>

/*Moddule d'alloction mémoire sécurisé*/

void *safe_alloc(int taille);
void safe_free(void *ptr);
void *safe_realloc(void *ptr, int taille);

#endif