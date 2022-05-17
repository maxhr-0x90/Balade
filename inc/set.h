#ifndef SET_H_
#define SET_H_

#define CELL_VOL 32 // 4 * 8

typedef struct set_s *set;
struct set_s {
  int *set;
  int cap;
};

set set_init(int size);
void set_free(set s);

void set_add(int i, set s);
void set_rem(int i, set s);

int set_has(int i, set s);
int set_is_empty(set s);
int set_size(set s);

void set_union(set a, set b, set out);
void set_inter(set a, set b, set out);

void print_set(set s);

#endif