#include "../inc/light.h"

void light_default(light *l){
  light_ambient(0, 0, 0, 1, l);
  light_diffuse(1, 1, 1, 1, l);
  light_specular(1, 1, 1, 1, l);
  light_position(0, 0, 1, 0, l);
  light_direction(0, 0, -1, l);
  light_spot_cutoff(180, l);
  light_spot_exponent(0, l);
  light_attenuation(CONSTANT, l);
}

void light_ambient(GLfloat r, GLfloat g, GLfloat b, GLfloat a, light *l){
  l->ambient[0] = r;
  l->ambient[1] = g;
  l->ambient[2] = b;
  l->ambient[3] = a;
}

void light_diffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a, light *l){
  l->diffuse[0] = r;
  l->diffuse[1] = g;
  l->diffuse[2] = b;
  l->diffuse[3] = a;
}

void light_specular(GLfloat r, GLfloat g, GLfloat b, GLfloat a, light *l){
  l->specular[0] = r;
  l->specular[1] = g;
  l->specular[2] = b;
  l->specular[3] = a;
}

void light_position(GLfloat x, GLfloat y, GLfloat z, GLfloat w, light *l){
  l->position[0] = x;
  l->position[1] = y;
  l->position[2] = z;
  l->position[3] = w;
}

void light_direction(GLfloat x, GLfloat y, GLfloat z, light *l){
  l->direction[0] = x;
  l->direction[1] = y;
  l->direction[2] = z;
}

void light_spot_cutoff(GLfloat cutoff, light *l){
  l->spot_cutoff = cutoff;
}

void light_spot_exponent(GLfloat expo, light *l){
  l->spot_exponent = expo;
}

void light_attenuation(attenuation att, light *l){
  l->attenuation = att;
}