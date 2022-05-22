/* Module gérant les transformation 2D */

#ifndef TRANSFORM_2D_H_
#define TRANSFORM_2D_H_

#include "GL/gl.h"
#include "GL/glut.h"

#include "vector.h"
#include "transform_3d.h"

typedef GLfloat trans_2d[9];

// Initialise la transformation 2D à l'identité
void trans2d_id(trans_2d mat);

// comb = cur * add
void trans2d_comb(trans_2d add, trans_2d cur, trans_2d comb);

// Multiplie la matrice actuelle par une matrice de translation
void trans2d_translate(GLfloat x, GLfloat y, trans_2d mat);

// Multiplie la matrice actuelle par une matrice de rotation
void trans2d_rotate(GLfloat angle, trans_2d mat);

// Multiplie la matrice actuelle par une matrice de mise à l'échelle
void trans2d_scale(GLfloat x, GLfloat y, trans_2d mat);

// Place l'inverse de src dans dest
int inverse2d(trans_2d src, trans_2d dest);

// Multiplie le vecteur 2D par la matrice de transformation
void transformv2(trans_2d tr, vector2f vec);

// Crée une matrice de transformation 3D en fonction d'une matrice de transformation 2D
void trans2d_to_3d(trans_2d in, trans_3d out);

// Affiche la matrice de transformation
void aff_trans_2d(trans_2d tr);

#endif