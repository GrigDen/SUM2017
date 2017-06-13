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
typedef struct tagDG5UNIT_CONTROL
{
  DG5_UNIT_BASE_FIELDS;
  HFONT hFont;           /* Using font */
} DG5UNIT_CONTROL;

/* Control unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       DG5UNIT_CONTROL *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitInit( DG5UNIT_CONTROL *Uni, dg5ANIM *Ani )
{
  Uni->hFont = CreateFont(30, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE, RUSSIAN_CHARSET,
    OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE, "");
} /* End of 'DG5_UnitInit' function */

/* Control unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       DG5UNIT_CONTROL *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitClose( DG5UNIT_CONTROL *Uni, dg5ANIM *Ani )
{
  DeleteObject(Uni->hFont);
} /* End of 'DG5_UnitClose' function */

/* Control unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       DG5UNIT_CONTROL *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitResponse( DG5UNIT_CONTROL *Uni, dg5ANIM *Ani )
{
  /* INT i; */

  if (Ani->KeysClick[VK_ESCAPE])
    DG5_AnimDoExit();
  else if (Ani->KeysClick['F'])
    DG5_AnimFlipFullScreen();
  else if (Ani->KeysClick['P'])
    DG5_Anim.IsPause = !DG5_Anim.IsPause;
  /*
  else if (Ani->KeysClick[VK_SPACE])
    for (i = 0; i < 30; i++)
      DG5_AnimAddUnit(DG5_UnitCreateBounceBall());
  else if (Ani->JBut[0])
    DG5_AnimAddUnit(DG5_UnitCreateBounceBall());
  */
} /* End of 'DG5_UnitResponse' function */

/* Control unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       DG5UNIT_CONTROL *Uni;
 *   - animation context:
 *       dg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID DG5_UnitRender( DG5UNIT_CONTROL *Uni, dg5ANIM *Ani )
{
  HFONT hFontOld = SelectObject(Ani->hDC, Uni->hFont);
  INT len;
  static CHAR Buf[100];

  len = sprintf(Buf, "FPS: %.5f, Units: %d, Wheel: %d"
    "J: %.3f %.3f %.3f %.3f",
    Ani->FPS, Ani->NumOfUnits, Ani->Mz,
    Ani->Jx, Ani->Jy, Ani->Jz, Ani->Jr);

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(0, 0, 0));
  TextOut(Ani->hDC, 2, 2, Buf, len);
  SetTextColor(Ani->hDC, RGB(0, 150, 0));
  TextOut(Ani->hDC, 0, 0, Buf, len);

  SelectObject(Ani->hDC, hFontOld);
} /* End of 'DG5_UnitRender' function */

/* Control unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (dg5UNIT *) pointer to created unit.
 */
dg5UNIT * DG5_UnitCreateControl( VOID )
{
  DG5UNIT_CONTROL *Uni;

  /* Memory allocation */
  if ((Uni = (DG5UNIT_CONTROL *)DG5_AnimUnitCreate(sizeof(DG5UNIT_CONTROL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)DG5_UnitInit;
  Uni->Close = (VOID *)DG5_UnitClose;
  Uni->Response = (VOID *)DG5_UnitResponse;
  Uni->Render = (VOID *)DG5_UnitRender;
  return (dg5UNIT *)Uni;
} /* End of 'DG5_UnitCreateControl' function */

/* END OF 'U_CONTR.C' FILE */
