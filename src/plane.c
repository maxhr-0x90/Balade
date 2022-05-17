#include "../inc/plane.h"
#include "../inc/alloc.h"

plane plane_init(vector3f norm, float d){
  plane p = safe_alloc(sizeof(struct plane_s));
  vec3_cpy(norm, p->n);
  p->d = d;

  return p;
}

void plane_free(plane p){
  if (p == NULL){ return; }

  safe_free(p);
}