/* Module gérant les k-arbres */

#ifndef KARBRE_H_
#define KARBRE_H_

// Élément de racine 
typedef void *element;

// Structure des k-arbres
typedef struct karbre *karbre;
struct karbre{
  element racine;
  int k;
  karbre *fils;
};

// Retrourne un arbre vide
karbre kArbreVide();

// Concatène k k-arbres sur une racine
karbre kConsArbre(int k, element racine, ...);

// Renvoie le ieme fils de l'arbre A
karbre kFils(int ieme, karbre A);

// Renvoie la racine de l'arbre A
element kRacine(karbre A);

// Retrourne 1 si le k-arbre est vide, 0 sinon
int kEstVide(karbre A);

// Affiche le k-arbre
void kAfficher(karbre A);

#endif