/* Module utilitaire pour OpneGL */

#ifndef GL_UTIL_H_
#define GL_UTIL_H_

#include "vector.h"
#include "transform_3d.h"

// Dessine une AABB
void draw_aabb(vector3f c000, vector3f c111);

// Dessine les axes x, y  et z à l'origine
void draw_axes(float length);

// Dessine un frustum
void draw_frustum(trans_3d proj);

// Dessine un cylindre
void draw_cylinder(vector3f center, float height, float radius, int div);

#endif