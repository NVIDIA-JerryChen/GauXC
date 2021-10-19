#ifndef __MY_INTEGRAL_1_1
#define __MY_INTEGRAL_1_1

#include "integral_1_1.h"

void integral_1_1(size_t npts,
                  shell_pair shpair,
                  point *points,
                  double *Xi,
                  double *Xj,
                  int stX,
                  int ldX,
                  double *Gi,
                  double *Gj,
                  int stG, 
                  int ldG, 
                  double *weights);

#endif
