/* FILE NAME: RENDER.C
 * PROGRAMMER: DG5
 * DATE: 10.06.2017
 * PURPOSE: Base rendering system module.
 */

#include "anim.h"

/* Project parameters */
DBL
  DG5_RndWp,       /* Project plane width */
  DG5_RndHp,       /* Project plane height */
  DG5_RndProjDist, /* Distance from viewer to project plane */
  DG5_RndProjSize; /* Prohect plane inner size */

MATR
  DG5_RndMatrView; /* Viewer matrix */

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DG5_RndInit( VOID )
{
  DG5_RndWp = 1;
  DG5_RndHp = 1;
  DG5_RndProjDist = 1;
  DG5_RndProjSize = 1;

  DG5_RndMatrView = MatrView(VecSet1(23), VecSet1(0), VecSet(0, 1, 0));
} /* End of 'DG5_RndInit' function */

/* Project parameters adjust function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DG5_RndSetProj( VOID )
{
  DG5_RndWp = DG5_RndProjSize;
  DG5_RndHp = DG5_RndProjSize;
  if (DG5_Anim.W > DG5_Anim.H)
    DG5_RndWp *= (DBL)DG5_Anim.W / DG5_Anim.H;
  else
    DG5_RndHp *= (DBL)DG5_Anim.H / DG5_Anim.W;
} /* End of 'DG5_RndSetProj' function */

/* END OF 'RENDER.C' FILE */
