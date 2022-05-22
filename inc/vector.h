/* Module gérant les vecteurs */

#ifndef VECTOR_H_
#define VECTOR_H_

#include "GL/gl.h"
#include "GL/glut.h"

typedef GLfloat vector3f[3];
typedef GLfloat vector2f[2];

GLfloat normv2(vector2f v2);
GLfloat normv3(vector3f v3);

void addv3(vector3f u, vector3f v, vector3f dest);
void subv3(vector3f u, vector3f v, vector3f dest);
void multv3(vector3f u, float k);

void addv2(vector2f u, vector2f v, vector2f dest);
void subv2(vector2f u, vector2f v, vector2f dest);
void multv2(vector2f u, float k);

void normalizev2(vector2f v2);
void normalizev3(vector3f v3);

GLfloat dotv2(vector2f vin1, vector2f vin2);
GLfloat dotv3(vector3f vin1, vector3f vin2);

void cross(vector3f vin1, vector3f vin2, vector3f vout);

void vec2_cpy(vector2f src, vector2f dest);
void vec3_cpy(vector3f src, vector3f dest);

#endif