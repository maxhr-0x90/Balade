#include "../inc/model_generator.h"
#include "../inc/model.h"
#include "../inc/alloc.h"
#include "../inc/vector.h"
#include "../inc/open-simplex-noise.h"

#include <math.h>

float heightmap(
  struct osn_context *ctx, 
  float x, float y, float scale, float scaleh,
  float lacunarity, float persistance, int octaves
){
  float height = 0.0f;
  for (int i = 0; i < octaves; i++){
    height += (float)open_simplex_noise2(
      ctx, 
      x/scale * powf(lacunarity, i), 
      y/scale * powf(lacunarity, i)
    ) * powf(persistance, i);
  }

  return height/scaleh;
}

model ground_model(float width, float height, int divw, int divh){
  model mod = model_init(1);

  material mat = {
    {0.5, 0.06, 0.11, 1.00},
    {0.43, 0.47, 0.54, 1.00},
    {0.33, 0.33, 0.52, 1.00},
    {0.00, 0.00, 0.00, 0.00},
    10.0,
    -1
  };

  mesh me = mesh_init(
    mat, 
    divw * divh * 2, 
    (divw + 1) * (divh + 1), 
    divw * divh * 2, 
    (divw + 1) * (divh + 1)
  );

  struct osn_context *ctx;

	open_simplex_noise(69420, &ctx);

  for (int y = 0; y <= divh; y++){
    for (int x = 0; x <= divw; x++){
      mesh_add_vert(vertex_init(
        ((float)x/divw - 0.5f) * width, 
        ((float)y/divh - 0.5f) * height, 
        heightmap(ctx, x, y, 1, 1, 1.5, 0.75f, 5)
      ), me);

      mesh_add_uvc(uvcoord_init((float)x/divw, (float)y/divh), me);
    }
  }

  for (int y = 0; y < divh; y++){
    for (int x = 0; x < divw; x++){
      vector3f v1, v2, n;
      vertex a, b, c;

      a = (vertex)array_get(y * (divw + 1) + x, me->vertices);
      b = (vertex)array_get((y + 1) * (divw + 1) + x, me->vertices);
      c = (vertex)array_get((y + 1) * (divw + 1) + x + 1, me->vertices);

      v1[0] = (*b)[0] - (*a)[0];
      v1[1] = (*b)[1] - (*a)[1];
      v1[2] = (*b)[2] - (*a)[2];

      v2[0] = (*c)[0] - (*a)[0];
      v2[1] = (*c)[1] - (*a)[1];
      v2[2] = (*c)[2] - (*a)[2];

      cross(v2, v1, n);
      normalizev3(n);

      mesh_add_norm(normal_init(n[0], n[1], n[2]), me);

      a = (vertex)array_get((y + 1) * (divw + 1) + x + 1, me->vertices);
      b = (vertex)array_get(y * (divw + 1) + x + 1, me->vertices);
      c = (vertex)array_get(y * (divw + 1) + x, me->vertices);

      v1[0] = (*b)[0] - (*a)[0];
      v1[1] = (*b)[1] - (*a)[1];
      v1[2] = (*b)[2] - (*a)[2];

      v2[0] = (*c)[0] - (*a)[0];
      v2[1] = (*c)[1] - (*a)[1];
      v2[2] = (*c)[2] - (*a)[2];

      cross(v2, v1, n);
      normalizev3(n);

      mesh_add_norm(normal_init(n[0], n[1], n[2]), me);

      mesh_add_face(face_init(
        y * (divw + 1) + x, 
        (y + 1) * (divw + 1) + x, 
        (y + 1) * (divw + 1) + x + 1,
        y * (divw + 1) + x, 
        (y + 1) * (divw + 1) + x, 
        (y + 1) * (divw + 1) + x + 1,
        (x + y * divw) * 2
      ), me);

      mesh_add_face(face_init(
        (y + 1) * (divw + 1) + x + 1,
        y * (divw + 1) + x + 1, 
        y * (divw + 1) + x, 
        (y + 1) * (divw + 1) + x + 1,
        y * (divw + 1) + x + 1, 
        y * (divw + 1) + x, 
        (x + y * divw) * 2 + 1
      ), me);
    }
  }
  

  model_add_part(me, mod);

  return mod;
}