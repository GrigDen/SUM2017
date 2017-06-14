/* FILE NAME: RENDER.C
 * PROGRAMMER: DG5
 * DATE: 10.06.2017
 * PURPOSE: Base rendering system module.
 */

#include "anim.h"

/* Project parameters */
DBL
  DG5_RndProjSize,       /* Prohect plane inner size */
  DG5_RndProjDist,     /* Distance from viewer to project plane */
  DG5_RndProjFarClip; /* distance to project far clip plane */

MATR
  DG5_RndMatrView, /* Viewer matrix */
  DG5_RndMatrProj; /* Projection Matrix */

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DG5_RndInit( VOID )
{
  DG5_RndProjDist = 1;
  DG5_RndProjSize = 1;
  DG5_RndProjFarClip = 1000;

  DG5_RndMatrView = MatrView(VecSet1(23), VecSet1(6), VecSet(0, 1, 0));
} /* End of 'DG5_RndInit' function */

/* Project parameters adjust function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DG5_RndSetProj( VOID )
{
  DBL rx = DG5_RndProjSize / 2, ry = DG5_RndProjSize / 2;

/*DG5_RndWp = DG5_RndProjSize;
  DG5_RndHp = DG5_RndProjSize;         */
  if (DG5_Anim.W > DG5_Anim.H)
    rx *= (DBL)DG5_Anim.W / DG5_Anim.H;
    /*DG5_RndWp *= (DBL)DG5_Anim.W / DG5_Anim.H;*/
  else
    ry *= (DBL)DG5_Anim.H / DG5_Anim.W;
    /* DG5_RndHp *= (DBL)DG5_Anim.H / DG5_Anim.W;*/
  DG5_RndMatrProj = MatrFrustum( -rx, rx, -ry, ry, DG5_RndProjDist, DG5_RndProjFarClip);
} /* End of 'DG5_RndSetProj' function */

/* END OF 'RENDER.C' FILE */