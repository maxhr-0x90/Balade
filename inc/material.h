#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "GL/gl.h"
#include "GL/glut.h"

typedef struct material {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat emission[4];
  GLfloat shininess;
  GLuint texture;
} material;

#endif