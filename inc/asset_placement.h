#ifndef ASSET_PLACEMENT_H_
#define ASSET_PLACEMENT_H_

#include "../inc/model.h"
#include "../inc/light.h"

void place_model(mod_inst mod);
void place_partial(partial part);
void render_mesh(mesh me);

void place_wire_model(mod_inst mod);
void place_wire_partial(partial part);
void render_wire_mesh(mesh me);

void place_light(light l, GLenum num);

#endif