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

karbre boule2arbre(int x, int y, int z, int r){
  return boule2arbre_bis(x, y, z, r, 1 << N, 0, 0, 0, 1 << N, 1 << N, 1 << N);
}

karbre boule2arbre_bis(
  int x, int y, int z, int r, int n, 
  int x1, int y1, int z1, int x2, int y2, int z2
){
  if (n == 1){
    return kConsArbre(
      8, VIDE, 
      kArbreVide(), kArbreVide(), kArbreVide(), kArbreVide(),
      kArbreVide(), kArbreVide(), kArbreVide(), kArbreVide()
    );
  }

  int r2 = carre(r);
  float racine2 = 1.4142135f;
  if (
    normecarre(x1 - x, y1 - y, z1 - z) <= r2 &&
    normecarre(x2 - x, y1 - y, z1 - z) <= r2 &&
    normecarre(x1 - x, y2 - y, z1 - z) <= r2 &&
    normecarre(x2 - x, y2 - y, z1 - z) <= r2 &&
    normecarre(x1 - x, y1 - y, z2 - z) <= r2 &&
    normecarre(x2 - x, y1 - y, z2 - z) <= r2 &&
    normecarre(x1 - x, y2 - y, z2 - z) <= r2 &&
    normecarre(x2 - x, y2 - y, z2 - z) <= r2
  ) {
    return kConsArbre(
      8, PLEIN, 
      kArbreVide(), kArbreVide(), kArbreVide(), kArbreVide(),
      kArbreVide(), kArbreVide(), kArbreVide(), kArbreVide()
    );
  } else if ( // (sqrt(2) / 2 * n + r)^2 < dist(centre_cube, centre_boule)^2
    (div2(carre(n)) + racine2 * n * r + r2) < normecarre(x - mid(x1, x2), y - mid(y1, y2), z - mid(z1, z2)) 
  ) {
    return kConsArbre(
      8, VIDE, 
      kArbreVide(), kArbreVide(), kArbreVide(), kArbreVide(),
      kArbreVide(), kArbreVide(), kArbreVide(), kArbreVide()
    );
  } else {
    return kConsArbre(
      8, SOMMET,
      boule2arbre_bis(
        x, y, z, r, div2(n), 
        x1, y1, mid(z1, z2), mid(x1, x2), mid(y1, y2), z2
      ),
      boule2arbre_bis(
        x, y, z, r, div2(n),
        mid(x1, x2), y1, mid(z1, z2), x2, mid(y1, y2), z2
      ),
      boule2arbre_bis(
        x, y, z, r, div2(n), 
        mid(x1, x2), y1, z1, x2, mid(y1, y2), mid(z1, z2)
      ),
      boule2arbre_bis(
        x, y, z, r, div2(n), 
        x1, y1, z1, mid(x1, x2), mid(y1, y2), mid(z1, z2)
      ),
      boule2arbre_bis(
        x, y, z, r, div2(n), 
        x1, mid(y1, y2), mid(z1, z2), mid(x1, x2), y2, z2
      ),
      boule2arbre_bis(
        x, y, z, r, div2(n), 
        mid(x1, x2), mid(y1, y2), mid(z1, z2), x2, y2, z2
      ),
      boule2arbre_bis(
        x, y, z, r, div2(n), 
        mid(x1, x2), mid(y1, y2), z1, x2, y2, mid(z1, z2)
      ),
      boule2arbre_bis(
        x, y, z, r, div2(n), 
        x1, mid(y1, y2), z1, mid(x1, x2), y2, mid(z1, z2)
      )
    );
  }
}

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

karbre intersection(karbre A, karbre B){
  if (kRacine(A) == VIDE || kRacine(B) == VIDE){
    return kConsArbre(
      8, VIDE, 
      kArbreVide(), kArbreVide(), kArbreVide(), kArbreVide(),
      kArbreVide(), kArbreVide(), kArbreVide(), kArbreVide());
  }

  if (kRacine(A) == PLEIN){ return volumeCopie(B); }

  if (kRacine(B) == PLEIN){ return volumeCopie(A); }

  return kConsArbre(
    8, SOMMET,
    intersection(kFils(0, A), kFils(0, B)),
    intersection(kFils(1, A), kFils(1, B)),
    intersection(kFils(2, A), kFils(2, B)),
    intersection(kFils(3, A), kFils(3, B)),
    intersection(kFils(4, A), kFils(4, B)),
    intersection(kFils(5, A), kFils(5, B)),
    intersection(kFils(6, A), kFils(6, B)),
    intersection(kFils(7, A), kFils(7, B))
  );
}

void affiche_cube(float x1, float y1, float z1, float x2, float y2, float z2){
  glBegin(GL_QUADS);

  glColor3f(0.9, 0.9, 0.9);
  glVertex3f(x1, y1, z2);
  glVertex3f(x1, y2, z2);
  glVertex3f(x2, y2, z2);
  glVertex3f(x2, y1, z2);

  glColor3f(0.8, 0.8, 0.8);
  glVertex3f(x1, y2, z1);
  glVertex3f(x2, y2, z1);
  glVertex3f(x2, y2, z2);
  glVertex3f(x1, y2, z2);

  glColor3f(0.7, 0.7, 0.7);
  glVertex3f(x2, y1, z1);
  glVertex3f(x2, y2, z1);
  glVertex3f(x2, y2, z2);
  glVertex3f(x2, y1, z2);

  glColor3f(0.6, 0.6, 0.6);
  glVertex3f(x1, y1, z1);
  glVertex3f(x1, y2, z1);
  glVertex3f(x1, y2, z2);
  glVertex3f(x1, y1, z2);

  glColor3f(0.5, 0.5, 0.5);
  glVertex3f(x1, y1, z1);
  glVertex3f(x2, y1, z1);
  glVertex3f(x2, y1, z2);
  glVertex3f(x1, y1, z2);

  glColor3f(0.4, 0.4, 0.4);
  glVertex3f(x1, y1, z1);
  glVertex3f(x1, y2, z1);
  glVertex3f(x2, y2, z1);
  glVertex3f(x2, y1, z1);
  
  glEnd();
}

void affiche_cube_fils(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b){
  glBegin(GL_LINES);
  glColor3f(r, g, b);

  glVertex3f(x1, y1, z1);
  glVertex3f(x2, y1, z1);

  glVertex3f(x1, y1, z1);
  glVertex3f(x1, y2, z1);

  glVertex3f(x1, y1, z1);
  glVertex3f(x1, y1, z2);

  glVertex3f(x2, y2, z2);
  glVertex3f(x1, y2, z2);

  glVertex3f(x2, y2, z2);
  glVertex3f(x2, y1, z2);

  glVertex3f(x2, y2, z2);
  glVertex3f(x2, y2, z1);

  glVertex3f(x2, y1, z1);
  glVertex3f(x2, y2, z1);

  glVertex3f(x2, y1, z1);
  glVertex3f(x2, y1, z2);

  glVertex3f(x1, y2, z1);
  glVertex3f(x2, y2, z1);

  glVertex3f(x1, y2, z1);
  glVertex3f(x1, y2, z2);

  glVertex3f(x1, y1, z2);
  glVertex3f(x2, y1, z2);

  glVertex3f(x1, y1, z2);
  glVertex3f(x1, y2, z2);

  glEnd();
}

void afficherVolumeRec(karbre V, float x1, float y1, float z1, float x2, float y2, float z2, int filaire){
  if (kEstVide(V)){ return; }

  if (V->racine == SOMMET){
    afficherVolumeRec(kFils(0, V), x1, y1, (z1 + z2)/2, (x1 + x2)/2, (y1 + y2)/2, z2, filaire);
    afficherVolumeRec(kFils(1, V), (x1 + x2)/2, y1, (z1 + z2)/2, x2, (y1 + y2)/2, z2, filaire);
    afficherVolumeRec(kFils(2, V), (x1 + x2)/2, y1, z1, x2, (y1 + y2)/2, (z1 + z2)/2, filaire);
    afficherVolumeRec(kFils(3, V), x1, y1, z1, (x1 + x2)/2, (y1 + y2)/2, (z1 + z2)/2, filaire);
    
    afficherVolumeRec(kFils(4, V), x1, (y1 + y2)/2, (z1 + z2)/2, (x1 + x2)/2, y2, z2, filaire);
    afficherVolumeRec(kFils(5, V), (x1 + x2)/2, (y1 + y2)/2, (z1 + z2)/2, x2, y2, z2, filaire);
    afficherVolumeRec(kFils(6, V), (x1 + x2)/2, (y1 + y2)/2, z1, x2, y2, (z1 + z2)/2, filaire);
    afficherVolumeRec(kFils(7, V), x1, (y1 + y2)/2, z1, (x1 + x2)/2, y2, (z1 + z2)/2, filaire);
  } else {
    if (V->racine){
      if (!filaire){
        affiche_cube(x1, y1, z1, x2, y2, z2);
      } else {
        affiche_cube_fils(x1, y1, z1, x2, y2, z2, 1, 1, 1);
      }
    } else {
      if (filaire){
        affiche_cube_fils(x1, y1, z1, x2, y2, z2, 0.5, 0, 0);
      }
    }
  }
}

void afficherVolume(karbre V, int filaire){
  afficherVolumeRec(V, -1, -1, -1, 1, 1, 1, filaire);
}