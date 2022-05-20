#include <math.h>

#include "../inc/player.h"
#include "../inc/alloc.h"

player player_init(){
  player p = safe_alloc(sizeof (struct player_s));

  player_set_pos(0, 0, 0, p);
  player_set_speed(0, 0, p);
  player_set_cam(0, 0, p);
  p->radius = .5f;
  p->act_radius = 1.0f;

  return p;
}

void player_free(player p){
  safe_free(p);
}

void player_set_pos(float x, float y, float z, player p){
  if (p == NULL){ return; }

  p->pos[0] = x;
  p->pos[1] = y;
  p->pos[2] = z;
}

void player_set_speed(float x, float y, player p){
  if (p == NULL){ return; }

  p->speed[0] = x;
  p->speed[1] = y;
}

void player_set_cam(float phi, float theta, player p){
  if (p == NULL){ return; }

  if (theta > M_PI){
    theta -= 2 * M_PI;
  } else if (theta < -M_PI){
    theta += 2 * M_PI;
  }

  if (phi < -M_PI_2 + 0.01){
    phi = -M_PI_2 + 0.01;
  } else if (phi > M_PI_2 - 0.01){
    phi = M_PI_2 - 0.01;
  }
  

  p->theta = theta;
  p->phi = phi;

  float cos_theta = cosf(theta);
  float sin_theta = sinf(theta);
  float cos_phi = cosf(phi);
  float sin_phi = sinf(phi);

  vector3f dir = {cos_theta * cos_phi, sin_theta * cos_phi, -sin_phi};
  vector3f right = {sin_theta, -cos_theta, 0};
  vector3f up = {cos_theta * sin_phi, sin_theta * sin_phi, cos_phi};

  vec3_cpy(dir, p->dir);
  vec3_cpy(right, p->right);
  vec3_cpy(up, p->up);
}

void step(player p){
  p->pos[0] += p->speed[0];
  p->pos[1] += p->speed[1];
}

void setup_torch(light *torch, player p){
  light_position(p->pos[0], p->pos[1], p->pos[2], 1, torch);
  light_direction(p->dir[0], p->dir[1], p->dir[2], torch);
}