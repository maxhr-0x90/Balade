#ifndef TRANSFORM_3D_H_
#define TRANSFORM_3D_H_

#include "GL/gl.h"
#include "GL/glut.h"

#include "../inc/vector.h"

typedef GLfloat trans_3d[16];

void trans_id(trans_3d mat);
void trans_comb(trans_3d add, trans_3d cur, trans_3d comb);
void trans_translate(GLfloat x, GLfloat y, GLfloat z, trans_3d mat);
void trans_rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z, trans_3d mat);
void trans_scale(GLfloat x, GLfloat y, GLfloat z, trans_3d mat);
int inverse(trans_3d src, trans_3d dest);
void transformv3(trans_3d tr, vector3f v);

#endif