/* Module définissant les hitboxs */

#ifndef HITBOX_H_
#define HITBOX_H_

#include "vector.h"
#include "transform_2d.h"

typedef enum {AABB, CIRCLE} box;
typedef enum {INNER, OUTER} check;
typedef enum {NO_HB, TREE, BORDER} hb_type;

// Structure d'une hitbox
typedef struct hitbox_s *hitbox;
struct hitbox_s {
  box shell;       // Forme de la hitbox
  check domain;    // Domaine de la hitbox (interne ou externe)
  hb_type type;    // Type de la hitbox
  float values[6]; // Valeur définissant la hitbox
  trans_2d mat;    // Matrice de transformation de la hitbox
};

// Initialise une hitbox
hitbox hitbox_init(box shell, check dom, hb_type type, float *vals);

// Libère l'espace occupé par une hitbox
void hitbox_free(hitbox hb);

// Vérifie s'il y a une collision entre une hitbox et un disque
int check_for_collision(vector2f pos, float radius, hitbox hb);

// Affiche une hitbox
void render_hitbox(hitbox hb, float zmin, float zmax);

#endif