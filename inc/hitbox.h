#ifndef HITBOX_H_
#define HITBOX_H_

#include "vector.h"
#include "transform_2d.h"

typedef enum {AABB, CIRCLE} box;
typedef enum {INNER, OUTER} check;

typedef struct hitbox_s *hitbox;
struct hitbox_s {
  box shell;
  check domain;
  float values[6];
  trans_2d mat;
};

hitbox hitbox_init(box shell, check dom, float *vals);
void hitbox_free(hitbox hb);
int check_for_collision(vector2f pos, float radius, hitbox hb);
void render_hitbox(hitbox hb, float zmin, float zmax);

#endif