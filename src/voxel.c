#include "GL/gl.h"
#include "GL/glut.h"

#include <stdio.h>
#include "../inc/voxel.h"

// Fonctions privées

int carre(int i){ return i * i; }

int normecarre(int x, int y, int z){
  return carre(x) + carre(y) + carre(z);
}

int div2(int i){ return i >> 1; }

int mult2(int i){ return i << 1; }

int mid(int a, int b){ return div2(a + b); }

karbre boule2arbre_bis(
  int x, int y, int z, int r, int n, 
  int x1, int y1, int z1, int x2, int y2, int z2
);

// Fonctions publiques

karbre volumeCopie(karbre A){
  if (kEstVide(A)){ return kArbreVide(); }
  return kConsArbre(
    8, kRacine(A),
    volumeCopie(kFils(0, A)),
    volumeCopie(kFils(1, A)),
    volumeCopie(kFils(2, A)),
    volumeCopie(kFils(3, A)),
    volumeCopie(kFils(4, A)),
    volumeCopie(kFils(5, A)),
    volumeCopie(kFils(6, A)),
    volumeCopie(kFils(7, A))
  );
}
