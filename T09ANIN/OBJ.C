/* FILE NAME: OBJ.C
 * PROGRAMMER: DG5
 * DATE: 14.06.2017
 * PURPOSE: Object handle module.
 */

#include <stdio.h>

#include "anim.h"

/* Create object function.
 * ARGUMENTS:
 *   - created object:
 *       dg5OBJ *Obj;
 *   - number of primitives:
 *       INT NumOfP;
* RETURNS: None.
 */
VOID DG5_RndObjCreate( dg5OBJ *Obj, INT NumOfP )
{
  memset(Obj, 0, sizeof(dg5PRIM));
  if ((Obj->P = malloc(sizeof(dg5PRIM) * NumOfP)) == NULL)
    return;
  memset(Obj->P, 0, sizeof(dg5PRIM) * NumOfP);
  Obj->NumOfP = NumOfP;
} /* Ens of 'DG5_RndObjCreate' function */

/* Free object function.
 * ARGUMENTS:
 *   - deleted object:
 *       dg5OBJ *Obj;
 * RETURNS: None.
 */
VOID DG5_RndObjFree( dg5OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfP; i++)
    DG5_RndPrimFree(&Obj->P[i]);
  free(Obj->P);
  memset(Obj, 0, sizeof(dg5PRIM));
} /* Ens of 'DG5_RndObjFree' function */

/* Draw primitive function.
 * ARGUMENTS:
 *   - drawing object:
 *       dg5OBJ *Obj;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID DG5_RndObjDraw( dg5OBJ *Obj, MATR M )
{
  INT i;

  for (i = 0; i < Obj->NumOfP; i++)
    DG5_RndPrimDraw(&Obj->P[i], M);
} /* Ens of 'DG5_RndObjDraw' function */

/* END OF 'OBJ.C' FILE */
