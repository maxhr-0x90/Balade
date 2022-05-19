#ifndef GL_UTIL_H_
#define GL_UTIL_H_

#include "vector.h"
#include "transform_3d.h"

void draw_aabb(vector3f c000, vector3f c111);
void draw_axes(float length);
void draw_frustum(trans_3d proj);
void draw_cylinder(vector3f center, float height, float radius, int div);

#endif