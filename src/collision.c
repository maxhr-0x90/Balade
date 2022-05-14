#include "../inc/collision.h"

#include <math.h>

float min2 (float f1, float f2);
float max2 (float f1, float f2);
float min3 (float f1, float f2, float f3);
float max3 (float f1, float f2, float f3);

int triangle_AABB_overlap(
  vector3f t0, vector3f t1, vector3f t2,
  vector3f c000, vector3f c111
){
  /* Mise en place des valeurs utiles */
  // Centre de la boite aligné avec les axes (BAA)
  vector3f center = {
    (c111[0] + c000[0]) / 2, (c111[1] + c000[1]) / 2, (c111[2] + c000[2]) / 2
  };

  // Vecteurs de moitié de longueur de la BAA (extentions)
  vector3f half = {
    c111[0] - center[0], c111[1] - center[1], c111[2] - center[2]
  };

  // Translation pour centre la BAA
  vector3f v0, v1, v2;
  vec3_cpy(t0, v0);
  vec3_cpy(t1, v1);
  vec3_cpy(t2, v2);

  v0[0] -= center[0];
  v0[1] -= center[1];
  v0[2] -= center[2];

  v1[0] -= center[0];
  v1[1] -= center[1];
  v1[2] -= center[2];
  
  v2[0] -= center[0];
  v2[1] -= center[1];
  v2[2] -= center[2];

  // BAA du triangle
  vector3f min_coord = {
    min3(v0[0], v1[0], v2[0]), min3(v0[1], v1[1], v2[1]), min3(v0[2], v1[2], v2[2])
  };
  vector3f max_coord = {
    max3(v0[0], v1[0], v2[0]), max3(v0[1], v1[1], v2[1]), max3(v0[2], v1[2], v2[2])
  };

  // Vecteurs des cotes du triangle
  vector3f f0 = {v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2]};
  vector3f f1 = {v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2]};
  vector3f f2 = {v0[0] - v2[0], v0[1] - v2[1], v0[2] - v2[2]};

  // Normale du triangle 
  vector3f normale;
  cross(f0, f1, normale);
  normv3(normale);

  // Produit vectoriel des cotes
  vector3f a00 = {0, -f0[2], f0[1]};
  vector3f a01 = {0, -f1[2], f1[1]};
  vector3f a02 = {0, -f2[2], f2[1]};
  vector3f a10 = {f0[2], 0, -f0[0]};
  vector3f a11 = {f1[2], 0, -f1[0]};
  vector3f a12 = {f2[2], 0, -f2[0]};
  vector3f a20 = {-f0[1], f0[0], 0};
  vector3f a21 = {-f1[1], f1[0], 0};
  vector3f a22 = {-f2[1], f2[0], 0};


  /* Étape 1 : intersection des BAAs (3 tests) */
  if (min_coord[0] > half[0] || max_coord[0] < -half[0]){ return 0; }
  if (min_coord[1] > half[1] || max_coord[1] < -half[1]){ return 0; }
  if (min_coord[2] > half[2] || max_coord[2] < -half[2]){ return 0; }

  /* Étape 2 : intersection plan - BAA (1 test) */
  // rayon de la projection
  float r = half[0] * fabsf(normale[0]) + half[1] * fabsf(normale[1]) + half[2] * fabsf(normale[2]);
  // d de l'équation du plan ax + by + cz - d = 0 décrit par le triangle
  float d = dotv3(v2, normale);
  if (abs(d) > r){ return 0; }

  /* Étape 3 : intersection des projection de la BAA et du triangle 
     sur les produits vectoriels des cotes (9 tests) */
  float p0, p1, p2; // Projection des points du triangle
  float radius; // Longueur de la projection des extentions
  

  radius = half[1] * fabsf(a00[1]) + half[2] * fabsf(a00[2]);
  p1 = dotv3(a00, v1);
  p2 = dotv3(a00, v2);
  if (min2(p1, p2) > radius || max2(p1, p2) < -radius){ return 0; }

  radius = half[1] * fabsf(a01[1]) + half[2] * fabsf(a01[2]);
  p0 = dotv3(a01, v0);
  p2 = dotv3(a01, v2);
  if (min2(p0, p2) > radius || max2(p0, p2) < -radius){ return 0; }

  radius = half[1] * fabsf(a02[1]) + half[2] * fabsf(a02[2]);
  p0 = dotv3(a02, v0);
  p1 = dotv3(a02, v1);
  if (min2(p0, p1) > radius || max2(p0, p1) < -radius){ return 0; }

  radius = half[0] * fabsf(a10[0]) + half[2] * fabsf(a10[2]);
  p1 = dotv3(a10, v1);
  p2 = dotv3(a10, v2);
  if (min2(p1, p2) > radius || max2(p1, p2) < -radius){ return 0; }

  radius = half[0] * fabsf(a11[0]) + half[2] * fabsf(a11[2]);
  p0 = dotv3(a11, v0);
  p2 = dotv3(a11, v2);
  if (min2(p0, p2) > radius || max2(p0, p2) < -radius){ return 0; }

  radius = half[0] * fabsf(a12[0]) + half[2] * fabsf(a12[2]);
  p0 = dotv3(a12, v0);
  p1 = dotv3(a12, v1);
  if (min2(p0, p1) > radius || max2(p0, p1) < -radius){ return 0; }

  radius = half[0] * fabsf(a20[0]) + half[1] * fabsf(a20[1]);
  p1 = dotv3(a20, v1);
  p2 = dotv3(a20, v2);
  if (min2(p1, p2) > radius || max2(p1, p2) < -radius){ return 0; }

  radius = half[0] * fabsf(a21[0]) + half[1] * fabsf(a21[1]);
  p0 = dotv3(a21, v0);
  p2 = dotv3(a21, v2);
  if (min2(p0, p2) > radius || max2(p0, p2) < -radius){ return 0; }

  radius = half[0] * fabsf(a22[0]) + half[1] * fabsf(a22[1]);
  p0 = dotv3(a22, v0);
  p1 = dotv3(a22, v1);
  if (min2(p0, p1) > radius || max2(p0, p1) < -radius){ return 0; }

  return 1;
}

float min2 (float f1, float f2){
  return f1 <= f2 ? f1 : f2;
}

float max2 (float f1, float f2){
  return f1 >= f2 ? f1 : f2;
}

float min3 (float f1, float f2, float f3){
  if (f1 <= f2){
    if (f1 <= f3){
      return f1;
    } else {
      return f3;
    }
  } else {
    if (f2 <= f3){
      return f2;
    } else {
      return f3;
    }
  }
}

float max3 (float f1, float f2, float f3){
  if (f1 >= f2){
    if (f1 >= f3){
      return f1;
    } else {
      return f3;
    }
  } else {
    if (f2 >= f3){
      return f2;
    } else {
      return f3;
    }
  }
}