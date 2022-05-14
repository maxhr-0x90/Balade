#include <string.h>

#include "../inc/gen_array.h"
#include "../inc/alloc.h"

// Initialise le tableau
array array_init(int capacity){
  array arr = safe_alloc(sizeof(struct array_s));

  arr->arr = safe_alloc(sizeof(elem) * capacity);
  arr->capacity = capacity;
  arr->size = 0;

  return arr;
}

// Libère l'espace occupé par le tableau
void array_free(int free_content, array arr){
  if (arr == NULL){ return; }

  if (free_content){ 
    for (int i = 0; i < arr->size; i++){
      safe_free(arr->arr[i]);
    }
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
    arr->arr = safe_realloc(arr->arr, sizeof(elem) * arr->capacity);
  }
}

// Fonction privée
// Vérifie et ajuste l'espace disponible du tableau
void check_capacity_inc(int inc, array arr){
  if (arr == NULL || inc <= 0){ return; }

  while (arr->capacity < arr->size + inc){ arr->capacity *= 2; }
  arr->arr = safe_realloc(arr->arr, sizeof(elem) * arr->capacity);
}

// Ajoute un element au tableau
void array_add(elem e, array arr){
  if (arr == NULL || e == NULL){ return; }

  check_capacity(arr);

  arr->arr[arr->size] = e;
  arr->size++;
}

void array_fill(int fill, array arr){
  if (arr == NULL || fill <= 0){ return; }

  check_capacity_inc(fill, arr);

  memset(&arr->arr[arr->size], 0, sizeof(elem) * fill);
  arr->size += fill;
}

// Renvoie un element du tableau
void *array_get(int i, array arr){
  if (arr == NULL || i < 0 || i >= arr->size) { return NULL; }

  return arr->arr[i];
}

void array_set(int i, elem e, array arr){
  if (arr == NULL || i < 0 || i >= arr->size) { return; }

  arr->arr[i] = e;
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