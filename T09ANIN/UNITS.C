/* FILE NAME: UNIT.C
 * PROGRAMMER: DG5
 * DATE: 09.06.17
 * PURPOSE: Units
 */

#include "Anim.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dg5UNIT *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitInit( dg5UNIT *Uni, dg5ANIM *Ani )
{
} /* End of 'DG5_UnitInit' function */
/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dg5UNIT *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitClose( dg5UNIT *Uni, dg5ANIM *Ani )
{
} /* End of 'DG5_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dg5UNIT *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitResponse( dg5UNIT *Uni, dg5ANIM *Ani )
{
} /* End of 'DG5_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dg5UNIT *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitRender( dg5UNIT *Uni, dg5ANIM *Ani )
{
} /* End of 'DG5_UnitRender' function */


/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (dg5UNIT *) pointer to created unit.
 */
dg5UNIT * DG5_AnimUnitCreate( INT Size )
{
  dg5UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(dg5UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = DG5_UnitInit;
  Uni->Close = DG5_UnitClose;
  Uni->Response = DG5_UnitResponse;
  Uni->Render = DG5_UnitRender;
  return Uni;
} /* End of 'DG5_AnimUnitCreate' function */

/* EOF 'UNITS.C' */