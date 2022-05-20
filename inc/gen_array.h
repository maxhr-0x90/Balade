#ifndef GEN_ARRAY_H_
#define GEN_ARRAY_H_

typedef void *elem;

typedef struct array_s *array;
struct array_s{
  elem *arr;
  int capacity;
  int size;
};


// initialise le tableau
array array_init(int capacity);

// libère l'espace occupé par le tableau seulement
void array_free(array arr);

// ajoute un element au tableau
void array_add(elem e, array arr);

void array_fill(int fill, array arr);

// renvoie un element du tableau
void *array_get(int i, array arr);

void array_set(int i, elem e, array arr);

// Renvoie le nombre d'elements
int array_size(array arr);

// Crée une copie du tableau
array array_copy(void *(*cpy_func)(void *), array src);

#endif