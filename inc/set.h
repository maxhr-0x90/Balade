/* Module gérant les ensembles */

#ifndef SET_H_
#define SET_H_

#define CELL_VOL 32 // 4 * 8

// Structure d'un ensemble
typedef struct set_s *set;
struct set_s {
  int *set;
  int cap;
};

// Initialise un ensemble
set set_init(int size);

// Libère l'espace occupé par un ensemble
void set_free(set s);


// Ajoute un élément à l'ensemble
void set_add(int i, set s);

// Retire un élément de l'ensemble
void set_rem(int i, set s);


// Renvoie 1 si l'ensemble s à l'élément i, 0 sinon
int set_has(int i, set s);

// Renvoie 1 si l'ensemble est vide, 0 sinon
int set_is_empty(set s);

// Renvoie le nombre d'éléments de l'ensemble
int set_size(set s);

// Place l'union de l'ensemble a et b dans l'ensemble out
void set_union(set a, set b, set out);

// Place l'intersection de l'ensemble a et b dans l'ensemble out
void set_inter(set a, set b, set out);


// Affiche un ensemble
void print_set(set s);

#endif