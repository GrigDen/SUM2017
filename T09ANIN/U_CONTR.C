/* FILE NAME: U_CONTR.C
 * PROGRAMMER: DG5
 * DATE: 10.06.2017
 * PURPOSE: Units base functions.
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "units.h"

/* Animation unit representation type */
typedef struct tagdg5UNIT_CONTROL
{
  DG5_UNIT_BASE_FIELDS
  DBL HRot, VRot;
  DBL Dist;
} dg5UNIT_CONTROL;

/* Control unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dg5UNIT_CONTROL *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitInit( dg5UNIT_CONTROL *Uni, dg5ANIM *Ani )
{
} /* End of 'DG5_UnitInit' function */

/* Control unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dg5UNIT_CONTROL *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitClose( dg5UNIT_CONTROL *Uni, dg5ANIM *Ani )
{
} /* End of 'DG5_UnitClose' function */

/* Control unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dg5UNIT_CONTROL *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitResponse( dg5UNIT_CONTROL *Uni, dg5ANIM *Ani )
{
  INT i;
  VEC V;

  if (Ani->KeysClick[VK_ESCAPE])
    DG5_AnimDoExit();
  else if (Ani->KeysClick['F'])
    DG5_AnimFlipFullScreen();
  else if (Ani->KeysClick['P'])
    DG5_Anim.IsPause = !DG5_Anim.IsPause;
  else if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  
  Uni->Dist += Ani->Mdz / 300.0;
  Uni->HRot -= 80 * 30 * Ani->GlobalDeltaTime * Ani->Keys[VK_LBUTTON] * Ani->Mdx ;
  Uni->VRot += 80 * 30 * Ani->GlobalDeltaTime * Ani->Keys[VK_LBUTTON] * Ani->Mdy;
  if (Uni->VRot > 89)
    Uni->VRot = 89;
  if (Uni->VRot < -89)
    Uni->VRot = -89; 
  ///* Erorr is here *///   
  V = VecSet(2, 2, Uni->Dist);
  V = VecMulMatr43(V, MatrMulMatr(MatrRotateX(Uni->VRot), MatrRotateY(Uni->HRot)));
  DG5_RndMatrView = MatrView(V, VecSet(0, 3, 0), VecSet(0, 1, 0));  

  DG5_RndLightPos = VecAddVec(DG5_RndLightPos, VecMulNum(VecSet(Ani->Jx, Ani->Jy, Ani->Jz), 8 * Ani->GlobalDeltaTime));
  if (Ani->JBut[2])
    DG5_RndLightPos = VecSet(0, 8, 0);
} /* End of 'DG5_UnitResponse' function */

/* Control unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       dg5UNIT_CONTROL *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitRender( dg5UNIT_CONTROL *Uni, dg5ANIM *Ani )
{
  static CHAR Buf[100];

  sprintf(Buf, "FPS: %.5f, Units: %d, Wheel: %d"
    "J: %.3f %.3f %.3f %.3f",
    Ani->FPS, Ani->NumOfUnits, Ani->Mz,
    Ani->Jx, Ani->Jy, Ani->Jz, Ani->Jr);

  SetWindowText(Ani->hWnd, Buf);
} /* End of 'DG5_UnitRender' function */

/* Control unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (dg5UNIT *) pointer to created unit.
 */
dg5UNIT * DG5_UnitCreateControl( VOID )
{
  dg5UNIT_CONTROL *Uni;

  /* Memory allocation */
  if ((Uni = (dg5UNIT_CONTROL *)DG5_AnimUnitCreate(sizeof(dg5UNIT_CONTROL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)DG5_UnitInit;
  Uni->Close = (VOID *)DG5_UnitClose;
  Uni->Response = (VOID *)DG5_UnitResponse;
  Uni->Render = (VOID *)DG5_UnitRender;
  return (dg5UNIT *)Uni;
} /* End of 'DG5_UnitCreateControl' function */

/* END OF 'U_CONTR.C' FILE */