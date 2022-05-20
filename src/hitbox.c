#include "../inc/hitbox.h"
#include "../inc/alloc.h"
#include "../inc/collision.h"
#include "../inc/gl_util.h"

hitbox hitbox_init(box shell, check dom, hb_type type, float *vals){
  hitbox hb = safe_alloc(sizeof(struct hitbox_s));
  hb->shell = shell;
  hb->domain = dom;
  hb->type = type;

  trans2d_id(hb->mat);

  int cpy_len = 0;
  switch (shell){
    case CIRCLE: cpy_len = 3; break;
    case AABB: cpy_len = 4; break;
  }

  for (int i = 0; i < cpy_len; i++){
    hb->values[i] = vals[i];
  }

  return hb;
}

void hitbox_free(hitbox hb){
  if (hb == NULL){ return; }

  safe_free(hb);
}

int check_for_collision(vector2f pos, float radius, hitbox hb){
  if (hb == NULL){ return 0; }

  switch (hb->shell){
  case CIRCLE:{
    vector2f c1 = {pos[0], pos[1]};
    vector2f c2 = {hb->values[0], hb->values[1]};
    if (hb->domain == INNER){
      return circle_circle_overlap(c1, radius, c2, hb->values[2]);
    } else {
      return !circle_circle_inside(c1, radius, c2, hb->values[2]);
    }
  }

  case AABB:{
    vector2f c = {pos[0], pos[1]};
    vector2f mid = {hb->values[0], hb->values[1]};
    vector2f h = {hb->values[2], hb->values[3]};
    if (hb->domain == INNER){
      return circle_aabb_overlap(c, radius, mid, h);
    } else {
      return !circle_aabb_inside(c, radius, mid, h);
    }
  }
  
  default: return 0;
  }
}

void render_hitbox(hitbox hb, float zmin, float zmax){
  switch (hb->shell){
  case CIRCLE: {
    vector2f c = {hb->values[0], hb->values[1]};
    vector3f center = {c[0], c[1], (zmax + zmin) / 2};
    trans_3d tr;
    trans2d_to_3d(hb->mat, tr);

    glPushMatrix();
    glLoadMatrixf(tr);
    draw_cylinder(center, zmax - zmin, hb->values[2], 16); break;
    glPopMatrix();
  }

  case AABB: {
    
    vector2f c = {hb->values[0], hb->values[1]};
    vector2f h = {hb->values[2], hb->values[3]};
    vector3f c000 = {c[0] - h[0], c[1] - h[1], zmin};
    vector3f c111 = {c[0] + h[0], c[1] + h[1], zmax};
    trans_3d tr;
    trans2d_to_3d(hb->mat, tr);
    
    glPushMatrix();
    glLoadMatrixf(tr);
    draw_aabb(c000, c111); break;
    glPopMatrix();
  }
  }
}