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
#include <math.h>

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
  model platform_m = load_model("models/platform.swag");
  model bridge_m = load_model("models/bridge.swag");

  forest_meshes_g = array_init(array_size(positions_g) * 2 + array_size(tree_linkage_g->edges) + 1);

  array_add(inst_init(ground_m), forest_meshes_g);

  for (int i = 0; i < array_size(positions_g); i++){
    coord pos = array_get(i, positions_g);

    mod_inst inst_pine = inst_init(pine_m);
    trans_translate(pos->x, pos->y, -3, inst_pine->mat);

    array_add(inst_pine, forest_meshes_g);

    mod_inst inst_platform = inst_init(platform_m);
    trans_translate(pos->x, pos->y, 22, inst_platform->mat);
    trans_scale(1.5, 1.5, 1, inst_platform->mat);

    array_add(inst_platform, forest_meshes_g);
  } 

  for (int i = 0; i < tree_linkage_g->nb_edges; i++){
    edge e = array_get(i, tree_linkage_g->edges);
    coord a = array_get(e->ext_a, tree_linkage_g->coords);
    coord b = array_get(e->ext_b, tree_linkage_g->coords);

    mod_inst inst_bridge = inst_init(bridge_m);

    float posx = (a->x + b->x) / 2;
    float posy = (a->y + b->y) / 2;
    float angle = atan2f(a->y - posy, a->x - posx);
    trans_translate(posx, posy, 22, inst_bridge->mat);
    trans_rotate(angle, 0, 0, 1, inst_bridge->mat);
    trans_scale(e->weight - 2, 1, 1, inst_bridge->mat);

    array_add(inst_bridge, forest_meshes_g);
  }
}

void generate_forest_hitbox(float side){
  float trunk_rad = .80f;
  float platform_rad = 2.75f;

  forest_hitboxes_ground_g = array_init(array_size(positions_g) + 1);
  forest_hitboxes_itp_g = array_init(array_size(positions_g) * 2 + array_size(tree_linkage_g->edges));

  float border[] = {0, 0, side / 2, side / 2};
  array_add(hitbox_init(AABB, OUTER, BORDER,  border), forest_hitboxes_ground_g);

  float hb_circle_vals[3];
  hitbox hb_tree;
  for (int i = 0; i < array_size(positions_g); i++) {
    coord pos = array_get(i, positions_g);

    hb_circle_vals[0] = pos->x;
    hb_circle_vals[1] = pos->y;
    hb_circle_vals[2] = trunk_rad;

    hb_tree = hitbox_init(CIRCLE, INNER, TREE, hb_circle_vals);
    array_add(hb_tree, forest_hitboxes_ground_g);

    hb_tree = hitbox_init(CIRCLE, INNER, TREE, hb_circle_vals);
    array_add(hb_tree, forest_hitboxes_itp_g);

    hb_circle_vals[2] = platform_rad;

    hb_tree = hitbox_init(CIRCLE, OUTER, BORDER, hb_circle_vals);
    array_add(hb_tree, forest_hitboxes_itp_g);
  }

  float hb_aabb_vals[4];
  hitbox hb_bridge;
  hb_aabb_vals[0] = 0;
  hb_aabb_vals[1] = 0;
  hb_aabb_vals[2] = 1;
  for (int i = 0; i < tree_linkage_g->nb_edges; i++){
    edge e = array_get(i, tree_linkage_g->edges);
    coord a = array_get(e->ext_a, tree_linkage_g->coords);
    coord b = array_get(e->ext_b, tree_linkage_g->coords);

    float posx = (a->x + b->x) / 2;
    float posy = (a->y + b->y) / 2;
    float angle = M_PI_2 - atan2f(a->y - posy, a->x - posx);

    hb_aabb_vals[3] = ( e->weight - 1 ) / 2;
    hb_bridge = hitbox_init(AABB, OUTER, BORDER, hb_aabb_vals);
    trans2d_translate(posx, posy, hb_bridge->mat);
    trans2d_rotate(angle, hb_bridge->mat);

    array_add(hb_bridge, forest_hitboxes_itp_g);
  }
}

array get_forest_meshes(){
  return forest_meshes_g;
}

array get_forest_ground_hitboxes(){
  return forest_hitboxes_ground_g;
}

array get_forst_itp_hitboxes(){
  return forest_hitboxes_itp_g;
}