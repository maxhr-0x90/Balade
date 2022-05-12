#include "../inc/gen_array.h"
#include "../inc/alloc.h"

// Initialise le tableau
array array_init(int capacity){
  array arr = safe_alloc(sizeof(struct array_s));

  arr->arr = safe_alloc(sizeof(void *) * capacity);
  arr->capacity = capacity;
  arr->size = 0;

  return arr;
}

// Libère l'espace occupé par le tableau
void array_free(array arr){
  if (arr == NULL){ return; }

  for (int i = 0; i < arr->size; i++){
    safe_free(arr->arr[i]);
  }
  
  safe_free(arr->arr);
  safe_free(arr);
}

// Fonction privée
// Vérifie et ajuste l'espace disponible du tableau
void check_capacity(array arr){
  if (arr == NULL){ return; }

  if (arr->capacity == arr->size){
    arr->capacity *= 2;
    arr->arr = safe_realloc(arr->arr, sizeof(void *) * arr->capacity);
  }
}

// Ajoute un element au tableau
void array_add(elem e, array arr){
  if (arr == NULL || e == NULL){ return; }

  check_capacity(arr);

  arr->arr[arr->size] = e;
  arr->size++;
}

// Renvoie un element du tableau
void *array_get(int i, array arr){
  if (arr == NULL || i < 0 || i >= arr->size) { return NULL; }

  return arr->arr[i];
}

// Renvoie le nombre d'elements
int array_size(array arr){
  if (arr == NULL){ return -1; }
  return arr->size;
}

// Crée une copie du tableau
array array_copy(void *(*cpy_func)(void *), array src){
  if (src == NULL){ return NULL; }

  array cpy = array_init(src->capacity);
  cpy->size = src->size;

  for (int i = 0; i < src->size; i++){
    cpy->arr[i] = cpy_func(src->arr[i]);
  }

  return cpy;
}