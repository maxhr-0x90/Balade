/* Module gérant les matériaux 3D */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "GL/gl.h"
#include "GL/glut.h"

// Structure d'un matériau
typedef struct material {
  GLfloat ambient[4];  // Couleur ambiente
  GLfloat diffuse[4];  // Couleur diffuse
  GLfloat specular[4]; // Couleur spéculaire
  GLfloat emission[4]; // Couleur émissive
  GLfloat shininess;   // Brillance
  GLuint texture;      // Texture
} material;

#endif