#ifndef KARBRE_H_
#define KARBRE_H_

#define VIDE 0
#define PLEIN 1
#define SOMMET -1

typedef int element;

#define ELEM_ERR -1

typedef struct karbre *karbre;
struct karbre{
  element racine;
  int k;
  karbre *fils;
};

karbre kArbreVide();
karbre kConsArbre(int k, element racine, ...);
karbre kFils(int ieme, karbre A);
element kRacine(karbre A);
int kEstVide(karbre A);
void kAfficher(karbre A);

#endif