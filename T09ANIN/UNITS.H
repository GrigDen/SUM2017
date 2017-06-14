#ifndef UNITS_H
#define UNITS_H

#include "def.h"

#define DG5_UNIT_BASE_FIELDS \
  VOID (*Init) (dg5UNIT *Uni, dg5ANIM *Ani); \
  VOID (*Close) (dg5UNIT *Uni, dg5ANIM *Ani); \
  VOID (*Response) (dg5UNIT *Uni, dg5ANIM *Ani); \
  VOID (*Render) (dg5UNIT *Uni, dg5ANIM *Ani);

#endif 