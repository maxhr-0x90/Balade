#ifndef TRANSFORM_3D_H_
#define TRANSFORM_3D_H_

#include "GL/gl.h"
#include "GL/glut.h"

typedef GLfloat trans_3d[16];

void trans_id(trans_3d mat);
void trans_translate(GLfloat x, GLfloat y, GLfloat z, trans_3d mat);
void trans_rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z, trans_3d mat);
void trans_scale(GLfloat x, GLfloat y, GLfloat z, trans_3d mat);

#endif