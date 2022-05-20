#include "../inc/map_generator.h"

#include "../inc/alloc.h"
#include "../inc/vector.h"
#include "../inc/graphe.h"
#include "../inc/open-simplex-noise.h"

#include "../inc/model.h"
#include "../inc/model_generator.h"
#include "../inc/model_loader.h"

#include "../inc/hitbox.h"

#include "../inc/controller.h"

#include <stdio.h>

array forest_meshes_g;
array forest_hitboxes_ground_g;
array forest_hitboxes_itp_g;
array positions_g;
graph tree_linkage_g;

array generate_tree_positions(int divx, int divy, int R, float density, float width, float height){
  float illegal_pos = 2.0f;
  struct osn_context *ctx = get_noise_context();

  float *bluenoise = safe_alloc(divx * divy * sizeof(float));
  array positions = array_init(divx * divy);

  for (int y = 0; y < divy; y++) {
    for (int x = 0; x < divx; x++) {
      float nx = (float)x/divx - 0.5f;
      float ny = (float)y/divy - 0.5f;

      bluenoise[x + y * divx] = (float)open_simplex_noise2(
        ctx, density * width * nx, density * height * ny
      ); 
    }
  }

  for (int yc = 0; yc < divy; yc++) {
    for (int xc = 0; xc < divx; xc++) {
      float max = 0;

      for (int yn = yc - R; yn <= yc + R; yn++) {
        for (int xn = xc - R; xn <= xc + R; xn++) {
          if (0 <= yn && yn < divy && 0 <= xn && xn < divx) {
            double e = bluenoise[yn * divx + xn];
            if (e > max) { max = e; }
          }
        }
      }

      float xpos = ((float)xc/divx - 0.5f) * width;
      float ypos = ((float)yc/divy - 0.5f) * height;
      if (
        bluenoise[yc * divx + xc] == max && 
        (xpos < -illegal_pos || xpos > illegal_pos) &&
        (ypos < -illegal_pos || ypos > illegal_pos)
      ) {
        coord c = coord_init(xpos, ypos);
        array_add(c, positions);
      }
    }
  }

  safe_free(bluenoise);
  return positions;
}

void generate_forest_mesh(float side, int div, int lod);
void generate_forest_hitbox(float side);

void generate_forest(float side, int div, int tree_sparsity, float tree_density, int lod){
  positions_g = generate_tree_positions(div * lod, div * lod, tree_sparsity, tree_density, side, side);

  tree_linkage_g = generate_MST(positions_g);

  generate_forest_mesh(side, div, lod);
  generate_forest_hitbox(side);
}

void generate_forest_mesh(float side, int div, int lod){
  model ground_m = ground_model(side, side, div, div, lod, lod);
  model pine_m = load_model("models/pine.swag");

  forest_meshes_g = array_init(array_size(positions_g) * 2 + array_size(tree_linkage_g->edges) + 1);

  array_add(inst_init(ground_m), forest_meshes_g);

  for (int i = 0; i < array_size(positions_g); i++){
    coord pos = array_get(i, positions_g);

    mod_inst inst_pine = inst_init(pine_m);
    trans_translate(pos->x, pos->y, -3, inst_pine->mat);

    array_add(inst_pine, forest_meshes_g);
  } 
}

void generate_forest_hitbox(float side){
  float trunk_rad = .80f;

  forest_hitboxes_ground_g = array_init(array_size(positions_g) + 1);

  float border[] = {0, 0, side, side};
  array_add(hitbox_init(AABB, OUTER, border), forest_hitboxes_ground_g);

  float hb_tree_vals[3];
  hb_tree_vals[2] = trunk_rad;
  for (int i = 0; i < array_size(positions_g); i++) {
    coord pos = array_get(i, positions_g);

    hb_tree_vals[0] = pos->x;
    hb_tree_vals[1] = pos->y;
    hitbox hb_tree = hitbox_init(CIRCLE, INNER, hb_tree_vals);

    array_add(hb_tree, forest_hitboxes_ground_g);
  }
}

array get_forest_meshes(){
  return forest_meshes_g;
}

array get_forest_ground_hitboxes(){
  return forest_hitboxes_ground_g;
}