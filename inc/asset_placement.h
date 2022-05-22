/* Module de placement des éléments 3D */

#ifndef ASSET_PLACEMENT_H_
#define ASSET_PLACEMENT_H_

#include "../inc/model.h"
#include "../inc/light.h"

// Place une instance de modèle
void place_model(mod_inst mod);
// Place une instance de modèle masquée
void place_partial(partial part);
// Rendu d'un mesh
void render_mesh(mesh me);

// Place une instance de modèle en fil de fer
void place_wire_model(mod_inst mod);
// Place une instance de modèle masquée en fil de fer
void place_wire_partial(partial part);
// Rendu d'un mesh fil de fer
void render_wire_mesh(mesh me);

// Place une lumière
void place_light(light l, GLenum num);

#endif