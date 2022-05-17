#include "../inc/frustum.h"
#include "../inc/alloc.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

frustum frustum_init(trans_3d proj, trans_3d view){
  trans_3d projview;
  frustum f;

  f = safe_alloc(sizeof (struct frustum_s));

  if (view != NULL){
    trans_comb(proj, view, projview);
  } else {
    memcpy(projview, proj, sizeof(trans_3d));
  }

  vector3f row1 = {projview[0], projview[4], projview[8]};
  vector3f row2 = {projview[1], projview[5], projview[9]};
  vector3f row3 = {projview[2], projview[6], projview[10]};
  vector3f row4 = {projview[3], projview[7], projview[11]};

  vector3f n_left, n_right, n_bottom, n_top, n_near, n_far;
  addv3(row4, row1, n_left);
  subv3(row4, row1, n_right);
  addv3(row4, row2, n_bottom);
  subv3(row4, row2, n_top);
  addv3(row4, row3, n_near);
  subv3(row4, row3, n_far);

  f->hs[0] = plane_init(n_left, projview[15] + projview[12]);
  f->hs[1] = plane_init(n_right, projview[15] - projview[12]);
  f->hs[2] = plane_init(n_bottom, projview[15] + projview[13]);
  f->hs[3] = plane_init(n_top, projview[15] - projview[13]);
  f->hs[4] = plane_init(n_near, projview[15] + projview[14]);
  f->hs[5] = plane_init(n_far, projview[15] - projview[14]);

  return f;
}

void frustum_free(frustum f){
  if (f == NULL){ return; }

  for (int i = 0; i < 6; i++){
    plane_free(f->hs[i]);
  }
  
  safe_free(f);
}

int frustum_AABB_intersect(frustum f, vector3f c000, vector3f c111){
  int ret = INSIDE; 
   vector3f vmin, vmax; 

   for(int i = 0; i < 6; ++i) { 
      // X axis 
      if(f->hs[i]->n[0] > 0) { 
        vmin[0] = c000[0]; 
        vmax[0] = c111[0]; 
      } else { 
        vmin[0] = c111[0];
        vmax[0] = c000[0]; 
      } 
      // Y axis 
      if(f->hs[i]->n[1] > 0) { 
        vmin[1] = c000[1]; 
        vmax[1] = c111[1]; 
      } else {
        vmin[1] = c111[1]; 
        vmax[1] = c000[1];
      } 
      // Z axis 
      if(f->hs[i]->n[2] > 0) {
        vmin[2] = c000[2]; 
        vmax[2] = c111[2];
      } else {
        vmin[2] = c111[2]; 
        vmax[2] = c000[2];
      } 

      if(dotv3(f->hs[i]->n, vmax) + f->hs[i]->d <= 0) 
         return OUTSIDE; 
      if(dotv3(f->hs[i]->n, vmin) + f->hs[i]->d <= 0) 
         ret = INTERSECT; 
   } 
   return ret;
}