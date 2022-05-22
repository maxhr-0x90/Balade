/* Module gérant les transformation 3D */

#ifndef TRANSFORM_3D_H_
#define TRANSFORM_3D_H_

#include "GL/gl.h"
#include "GL/glut.h"

#include "vector.h"


typedef GLfloat trans_3d[16];

// Initialise la transformation 3D à l'identité
void trans_id(trans_3d mat);

// comb = cur * add
void trans_comb(trans_3d add, trans_3d cur, trans_3d comb);

// Multiplie la matrice actuelle par une matrice de translation
void trans_translate(GLfloat x, GLfloat y, GLfloat z, trans_3d mat);

// Multiplie la matrice actuelle par une matrice de rotation
void trans_rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z, trans_3d mat);

// Multiplie la matrice actuelle par une matrice de mise à l'échelle
void trans_scale(GLfloat x, GLfloat y, GLfloat z, trans_3d mat);

// Place l'inverse de src dans dest
int inverse(trans_3d src, trans_3d dest);

// Multiplie le vecteur 2D par la matrice de transformation
void transformv3(trans_3d tr, vector3f v);

// Affiche la matrice de transformation
void print_trans_3d(trans_3d tr);

#endif