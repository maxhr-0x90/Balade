/* Module de gestion de tableau dynamique */

#ifndef GEN_ARRAY_H_
#define GEN_ARRAY_H_

/* Élément du tableau */
typedef void *elem;

/* Structure d'un tableau dynamique */
typedef struct array_s *array;
struct array_s{
  elem *arr;
  int capacity;
  int size;
};


// Initialise le tableau
array array_init(int capacity);

// Libère l'espace occupé par le tableau seulement
void array_free(array arr);

// Ajoute un element au tableau
void array_add(elem e, array arr);

// Rempli le tableau
void array_fill(int fill, array arr);

// Renvoie un element du tableau
elem array_get(int i, array arr);

void array_set(int i, elem e, array arr);

// Renvoie le nombre d'elements
int array_size(array arr);

// Crée une copie du tableau
array array_copy(void *(*cpy_func)(void *), array src);

#endif