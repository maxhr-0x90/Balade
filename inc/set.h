#ifndef SET_H_
#define SET_H_

typedef struct set_s *set;
struct set_s {
  int *set;
  int cap;
};


#endif