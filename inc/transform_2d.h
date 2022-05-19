#ifndef TRANSFORM_2D_H_
#define TRANSFORM_2D_H_

#include "GL/gl.h"
#include "GL/glut.h"

#include "vector.h"
#include "transform_3d.h"

typedef GLfloat trans_2d[9];

void trans2d_id(trans_2d mat);
void trans2d_comb(trans_2d add, trans_2d cur, trans_2d comb);
void trans2d_translate(GLfloat x, GLfloat y, trans_2d mat);
void trans2d_rotate(GLfloat angle, trans_2d mat);
void trans2d_scale(GLfloat x, GLfloat y, trans_2d mat);
int inverse2d(trans_2d src, trans_2d dest);
void aff_trans_2d(trans_2d tr);
void transformv2(trans_2d tr, vector2f vec);
void trans2d_to_3d(trans_2d in, trans_3d out);

#endif