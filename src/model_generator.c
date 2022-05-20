#include "../inc/model_generator.h"
#include "../inc/model.h"
#include "../inc/alloc.h"
#include "../inc/vector.h"
#include "../inc/open-simplex-noise.h"
#include "../inc/controller.h"

#include <math.h>

void set_mesh_properties(mesh me, int divw, int divh);

mesh ground_mesh(
  material mat, struct osn_context *ctx,
  float width, float height, int divw, int divh, int offsetx, int offsety,
  int subdivw, int subdivh
);

float heightmap(
  struct osn_context *ctx, 
  float x, float y, float scale, float amplitude,
  float lacunarity, float persistance, int octaves
){
  float height = 0.0f;
  for (int i = 0; i < octaves; i++){
    height += (float)open_simplex_noise2(
      ctx, 
      x * scale * powf(lacunarity, i), 
      y * scale * powf(lacunarity, i)
    ) * powf(persistance, i);
  }

  return height * amplitude;
}

model ground_model(
  float width, float height, int divw, int divh, int subdivw, int subdivh
){
  model mod = model_init(divw * divh);

  material mat = {
    {0.5, 0.06, 0.11, 1.00},
    {0.43, 0.47, 0.54, 1.00},
    {0.33, 0.33, 0.52, 1.00},
    {0.00, 0.00, 0.00, 0.00},
    10.0,
    -1
  };

  struct osn_context *ctx = get_noise_context();

  for (int y = 0; y < divh; y++){
    for (int x = 0; x < divw; x++){
      mesh me = ground_mesh(
        mat, ctx, width, height, divw, divh, x, y, subdivw, subdivh
      );
      model_add_part(me, mod);
    }
  }

  open_simplex_noise_free(ctx);
  return mod;
}

mesh ground_mesh(
  material mat, struct osn_context *ctx,
  float width, float height, int divw, int divh, int offsetx, int offsety,
  int subdivw, int subdivh
){
  float scale = 1;
  float amplitude = 1;
  float lacunarity = 1.5f;
  float persistance = 0.75f;
  int octaves = 5;

  mesh me = mesh_init(
    mat, 
    subdivw * subdivh * 2, 
    (subdivw + 1) * (subdivh + 1), 
    subdivw * subdivh * 2, 
    (subdivw + 1) * (subdivh + 1)
  );

  int squaresw = divw * subdivw;
  int squaresh = divh * subdivh;

  for (int y = 0; y <= subdivh; y++){
    for (int x = 0; x <= subdivw; x++){
      float vert_x = ((float)(x + offsetx * subdivw)/squaresw - 0.5f) * width;
      float vert_y = ((float)(y + offsety * subdivh)/squaresh - 0.5f) * height;

      mesh_add_vert(vertex_init(
        vert_x, vert_y, 
        heightmap(
          ctx, vert_x, vert_y, scale, amplitude, lacunarity, persistance, octaves
        )
      ), me);

      mesh_add_uvc(uvcoord_init(vert_x, vert_y), me);
    }
  }

  set_mesh_properties(me, subdivw, subdivh);

  return me;
}

void set_mesh_properties(mesh me, int divw, int divh){
  for (int y = 0; y < divh; y++){
    for (int x = 0; x < divw; x++){
      vector3f v1, v2, n;
      vertex a, b, c;

      // Normale triangle 1
      a = (vertex)array_get(y * (divw + 1) + x, me->vertices);
      b = (vertex)array_get((y + 1) * (divw + 1) + x, me->vertices);
      c = (vertex)array_get((y + 1) * (divw + 1) + x + 1, me->vertices);

      v1[0] = b->x - a->x;
      v1[1] = b->y - a->y;
      v1[2] = b->z - a->z;

      v2[0] = c->x - a->x;
      v2[1] = c->y - a->y;
      v2[2] = c->z - a->z;

      cross(v2, v1, n);
      normalizev3(n);

      mesh_add_norm(normal_init(n[0], n[1], n[2]), me);

      // Normale triangle 2
      a = (vertex)array_get((y + 1) * (divw + 1) + x + 1, me->vertices);
      b = (vertex)array_get(y * (divw + 1) + x + 1, me->vertices);
      c = (vertex)array_get(y * (divw + 1) + x, me->vertices);

      v1[0] = b->x - a->x;
      v1[1] = b->y - a->y;
      v1[2] = b->z - a->z;

      v2[0] = c->x - a->x;
      v2[1] = c->y - a->y;
      v2[2] = c->z - a->z;

      cross(v2, v1, n);
      normalizev3(n);

      mesh_add_norm(normal_init(n[0], n[1], n[2]), me);

      // Face triangle 1
      mesh_add_face(face_init(
        y * (divw + 1) + x, 
        (y + 1) * (divw + 1) + x, 
        (y + 1) * (divw + 1) + x + 1,
        y * (divw + 1) + x, 
        (y + 1) * (divw + 1) + x, 
        (y + 1) * (divw + 1) + x + 1,
        (x + y * divw) * 2
      ), me);

      // Face triangle 2
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
}