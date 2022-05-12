#ifndef LIGHT_H_
#define LIGHT_H_

#include "GL/gl.h"
#include "GL/glut.h"

typedef enum {CONSTANT, LINEAR, QUADRATIQUE} attenuation;

typedef struct light {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat position[4];
  GLfloat direction[3];
  GLfloat spot_cutoff;
  GLfloat spot_exponent;
  attenuation attenuation;
} light;

void light_default(light *l);
void light_ambient(GLfloat r, GLfloat g, GLfloat b, GLfloat a, light *l);
void light_diffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a, light *l);
void light_specular(GLfloat r, GLfloat g, GLfloat b, GLfloat a, light *l);
void light_position(GLfloat x, GLfloat y, GLfloat z, GLfloat w, light *l);
void light_direction(GLfloat x, GLfloat y, GLfloat z, light *l);
void light_spot_cutoff(GLfloat cutoff, light *l);
void light_spot_exponent(GLfloat expo, light *l);
void light_attenuation(attenuation att, light *l);

#endif