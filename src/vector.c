#include <math.h>

#include "../inc/vector.h"

GLfloat normv2(vector2f v2){
  return sqrtf(v2[0]*v2[0] + v2[1]*v2[1]);
}

GLfloat normv3(vector3f v3){
  return sqrtf(v3[0]*v3[0] + v3[1]*v3[1] + v3[2]*v3[2]);
}

void addv3(vector3f u, vector3f v, vector3f dest){
  vector3f d = {u[0] + v[0], u[1] + v[1], u[2] + v[2]};
  vec3_cpy(d, dest);
}

void subv3(vector3f u, vector3f v, vector3f dest){
  vector3f d = {u[0] - v[0], u[1] - v[1], u[2] - v[2]};
  vec3_cpy(d, dest);
}

void multv3(vector3f u, float k){
  vector3f v = {u[0] * k, u[1] * k, u[2] * k};
  vec3_cpy(v, u);
}

void addv2(vector2f u, vector2f v, vector2f dest){
  vector2f d = {u[0] + v[0], u[1] + v[1]};
  vec2_cpy(d, dest);
}

void subv2(vector2f u, vector2f v, vector2f dest){
  vector2f d = {u[0] - v[0], u[1] - v[1]};
  vec2_cpy(d, dest);
}

void multv2(vector2f u, float k){
  vector2f v = {u[0] * k, u[1] * k};
  vec2_cpy(v, u);
}

// GLfloat Q_rsqrt(GLfloat number){
// 	long i;
// 	GLfloat x2, y;
// 	const GLfloat threehalfs = 1.5f;

// 	x2 = number * 0.5f;
// 	y = number;
// 	i = *(long *) &y; // evil floating point bit level hacking
// 	i = 0x5f3759df - (i >> 1); // what the fuck?
// 	y = *(GLfloat *) &i;
// 	y = y * (threehalfs - (x2 * y * y)); // 1st iteration
//   // y = y * (threehalfs - (x2 * y * y)); // 2nd iteration, this can be removed

// 	return y;
// }

GLfloat rsqrt(GLfloat n){
  return 1 / sqrt(n);
}

void normalizev2(vector2f v2){
  GLfloat norm2 = v2[0]*v2[0] + v2[1]*v2[1];
  v2[0] *= rsqrt(norm2);
  v2[1] *= rsqrt(norm2);
}

void normalizev3(vector3f v3){
  GLfloat norm2 = v3[0]*v3[0] + v3[1]*v3[1] + v3[2]*v3[2];
  v3[0] *= rsqrt(norm2);
  v3[1] *= rsqrt(norm2);
  v3[2] *= rsqrt(norm2);
}

GLfloat dotv2(vector2f u, vector2f v){
  return (u[0] * v[0] + u[1] * v[1]);
}

GLfloat dotv3(vector3f u, vector3f v){
  return (u[0] * v[0] + u[1] * v[1] + u[2] * v[2]);
}

void cross(vector3f u, vector3f v, vector3f vout){
  GLfloat x, y, z;

  x = u[1] * v[2] - u[2] * v[1];
  y = u[2] * v[0] - u[0] * v[2];
  z = u[0] * v[1] - u[1] * v[0];

  vout[0] = x;
  vout[1] = y;
  vout[2] = z;
}

void vec2_cpy(vector2f src, vector2f dest){
  dest[0] = src[0];
  dest[1] = src[1];
}

void vec3_cpy(vector3f src, vector3f dest){
  dest[0] = src[0];
  dest[1] = src[1];
  dest[2] = src[2];
}