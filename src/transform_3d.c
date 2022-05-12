#include "../inc/transform_3d.h"

#include "../inc/vector.h"

#include <string.h>
#include <math.h>

void trans_id(trans_3d mat){
  memset(mat, 0, sizeof (trans_3d));
  mat[0] = 1;
  mat[5] = 1;
  mat[10] = 1;
  mat[15] = 1;
}

void trans_comb(trans_3d add, trans_3d cur, trans_3d comb){
  // 1ere colonne
  comb[0]  = add[0] * cur[0] + add[1] * cur[4] + add[2] * cur[8] + add[3] * cur[12];
  comb[1]  = add[0] * cur[1] + add[1] * cur[5] + add[2] * cur[9] + add[3] * cur[13];
  comb[2]  = add[0] * cur[2] + add[1] * cur[6] + add[2] * cur[10] + add[3] * cur[14];
  comb[3]  = add[0] * cur[3] + add[1] * cur[7] + add[2] * cur[11] + add[3] * cur[15];

  // 2eme colonne
  comb[4]  = add[4] * cur[0] + add[5] * cur[4] + add[6] * cur[8] + add[7] * cur[12];
  comb[5]  = add[4] * cur[1] + add[5] * cur[5] + add[6] * cur[9] + add[7] * cur[13];
  comb[6]  = add[4] * cur[2] + add[5] * cur[6] + add[6] * cur[10] + add[7] * cur[14];
  comb[7]  = add[4] * cur[3] + add[5] * cur[7] + add[6] * cur[11] + add[7] * cur[15];

  // 3eme colonne
  comb[8]  = add[8] * cur[0] + add[9] * cur[4] + add[10] * cur[8] + add[11] * cur[12];
  comb[9]  = add[8] * cur[1] + add[9] * cur[5] + add[10] * cur[9] + add[11] * cur[13];
  comb[10] = add[8] * cur[2] + add[9] * cur[6] + add[10] * cur[10] + add[11] * cur[14];
  comb[11] = add[8] * cur[3] + add[9] * cur[7] + add[10] * cur[11] + add[11] * cur[15];

  // 4eme colonne
  comb[12] = add[12] * cur[0] + add[13] * cur[4] + add[14] * cur[8] + add[15] * cur[12];
  comb[13] = add[12] * cur[1] + add[13] * cur[5] + add[14] * cur[9] + add[15] * cur[13];
  comb[14] = add[12] * cur[2] + add[13] * cur[6] + add[14] * cur[10] + add[15] * cur[14];
  comb[15] = add[12] * cur[3] + add[13] * cur[7] + add[14] * cur[11] + add[15] * cur[15];
}

void trans_translate(GLfloat x, GLfloat y, GLfloat z, trans_3d mat){
  trans_3d comb, trans;
  
  trans_id(trans);
  trans[12] = x;
  trans[13] = y;
  trans[14] = z;

  trans_comb(trans, mat, comb);
  memcpy(mat, comb, sizeof (trans_3d));
}

void trans_rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z, trans_3d mat){
  trans_3d comb, trans;

  vector3f vec = {x, y, z};
  normalizev3(vec);

  GLfloat c = cosf(angle);
  GLfloat s = sinf(angle);
  
  // Rodrigues' Rotation Formula
  trans_id(trans);
  trans[0] = c + x * x * (1 - c);
  trans[1] = z * s + x * y * (1 - c);
  trans[2] = -y * s + x * z * (1 - c);

  trans[4] = x * y * (1 - c) - z * s;
  trans[5] = c + y * y * (1 - c);
  trans[6] = x * s + y * z * (1 - c);

  trans[8] = y * s + x * z * (1 - c);
  trans[9] = -x * s + y * z * (1 - c);
  trans[10] = c + z * z * (1 - c);

  trans_comb(trans, mat, comb);
  memcpy(mat, comb, sizeof (trans_3d));
}

void trans_scale(GLfloat x, GLfloat y, GLfloat z, trans_3d mat){
  trans_3d comb, trans;
  
  trans_id(trans);
  trans[0] = x;
  trans[5] = y;
  trans[10] = z;

  trans_comb(trans, mat, comb);
  memcpy(mat, comb, sizeof (trans_3d));
}