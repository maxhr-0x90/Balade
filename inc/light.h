/* Module gérant la lumière */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "GL/gl.h"
#include "GL/glut.h"

// Structure d'une lumière
typedef struct light {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat position[4];
  GLfloat direction[3];
  GLfloat spot_cutoff;
  GLfloat spot_exponent;
  GLfloat constant_att;
  GLfloat linear_att;
  GLfloat quadratic_att;
} light;

// Initialise une lumière par défaut
void light_default(light *l);

// Défini la couleur ambiante de la lumière
void light_ambient(GLfloat r, GLfloat g, GLfloat b, GLfloat a, light *l);

// Défini la couleur diffuse de la lumière
void light_diffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a, light *l);

// Défini la couleur spéculaire de la lumière
void light_specular(GLfloat r, GLfloat g, GLfloat b, GLfloat a, light *l);

// Défini la position de la lumière
void light_position(GLfloat x, GLfloat y, GLfloat z, GLfloat w, light *l);

// Défini la direction de la lumière
void light_direction(GLfloat x, GLfloat y, GLfloat z, light *l);

// Défini l'angle de diffusion de la lumière
void light_spot_cutoff(GLfloat cutoff, light *l);

// Défini l'attenuation en fonction de l'angle de la lumière
void light_spot_exponent(GLfloat expo, light *l);

// Défini l'attenuation en fonction de la distance de la lumière
void light_attenuation(GLfloat cnst, GLfloat line, GLfloat quad, light *l);

#endif