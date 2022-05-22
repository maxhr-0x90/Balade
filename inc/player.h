/* Module gérant le joueur */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "vector.h"
#include "light.h"

// Strucutre du joueur
typedef struct player_s *player;
struct player_s{
  vector3f dir, up, right, pos;
  vector2f speed;
  float phi, theta;
  float radius;
};

// Initialise le joueur
player player_init();

// Libère l'espace occupé par le joueur
void player_free(player p);

// Défini la position du joueur
void player_set_pos(float x, float y, float z, player p);

// Défini la vitesse du joueur
void player_set_speed(float x, float y, player p);

// Défini la vision du joueur
void player_set_cam(float phi, float theta, player p);

// Avance le joueur en fonction de la vitesse
void step(player p);

// Défini la lumière du joueur
void setup_torch(light *torch, player p);

#endif