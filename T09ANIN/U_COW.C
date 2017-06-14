/* FILE NAME: U_COW.C
 * PROGRAMMER: DG5
 * DATE: 10.06.2017
 * PURPOSE: Units base functions.
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "anim.h"

/* Animation unit representation type */
typedef struct tagdg5UNIT_COW
{
  DG5_UNIT_BASE_FIELDS
  dg5OBJ3D Cow; /* Cow model */
} dg5UNIT_COW;

/* Cow drawing unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dg5UNIT_COW *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitInit( dg5UNIT_COW *Uni, dg5ANIM *Ani )
{
  DG5_RndObjLoad(&Uni->Cow, "cow.object");
} /* End of 'DG5_UnitInit' function */

/* Cow drawing unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dg5UNIT_COW *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitClose( dg5UNIT_COW *Uni, dg5ANIM *Ani )
{
  DG5_RndObjFree(&Uni->Cow);
} /* End of 'DG5_UnitClose' function */

/* Cow drawing unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dg5UNIT_COW *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitResponse( dg5UNIT_COW *Uni, dg5ANIM *Ani )
{
} /* End of 'DG5_UnitResponse' function */

/* Cow drawing unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dg5UNIT_COW *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitRender( dg5UNIT_COW *Uni, dg5ANIM *Ani )
{
  DG5_RndObjDraw(&Uni->Cow, MatrMulMatr(MatrRotateY(Ani->Time * 100), MatrTranslate(VecSet1((DBL)Ani->Mz / 100))));
} /* End of 'DG5_UnitRender' function */

/* Cow drawing unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (dg5UNIT *) pointer to created unit.
 */
dg5UNIT * DG5_UnitCreateCow( VOID )
{
  dg5UNIT_COW *Uni;

  /* Memory allocation */
  if ((Uni = (dg5UNIT_COW *)DG5_AnimUnitCreate(sizeof(dg5UNIT_COW))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)DG5_UnitInit;
  Uni->Close = (VOID *)DG5_UnitClose;
  Uni->Response = (VOID *)DG5_UnitResponse;
  Uni->Render = (VOID *)DG5_UnitRender;
  return (dg5UNIT *)Uni;
} /* End of 'DG5_UnitCreateCow' function */

/* END OF 'U_COW.C' FILE */