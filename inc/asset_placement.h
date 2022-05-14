#ifndef ASSET_PLACEMENT_H_
#define ASSET_PLACEMENT_H_

#include "../inc/model.h"
#include "../inc/light.h"

void place_model(mod_inst mod);
void place_wire_model(mod_inst mod);
void place_light(light l, GLenum num);

#endif