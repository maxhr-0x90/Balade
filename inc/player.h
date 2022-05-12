#ifndef PLAYER_H_
#define PLAYER_H_

#include "vector.h"

typedef struct player_s *player;
struct player_s{
  vector3f dir, up, right, pos;
  vector2f speed;
  float phi, theta;
};

player player_init();

void player_free(player p);

void player_set_pos(float x, float y, float z, player p);

void player_set_speed(float x, float y, player p);

void player_set_cam(float phi, float theta, player p);

void step(float delta, player p);

#endif