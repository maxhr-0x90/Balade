#ifndef VOXEL_H_
#define VOXEL_H_

#include "k-arbre.h"

#define N 6

karbre boule2arbre(int x, int y, int z, int r);
karbre intersection(karbre A, karbre B);
karbre volumeCopie(karbre A);

void affiche_cube(float x1, float y1, float z1, float x2, float y2, float z2);
void affiche_cube_fils(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b);
void afficherVolume(karbre V, int filaire);

#endif