/* FILE NAME: Vec.C
 * PROGRAMMER: GD5
 * DATE: 07.06.2017
 * PURPOSE: drawing sphere.
 */
#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"
#include "units.h"

#define DG5_MAX_UNITS 50

typedef struct tagdg5ANIM dg5ANIM;
typedef struct tagdg5UNIT dg5UNIT;

struct tagdg5UNIT
{
  dg5_UNIT_BASE_FIELDS
};

struct tagdg5ANIM
{
  HWND hWnd;
  HDC hDC;
  INT W, H;
  HBITMAP hFrame;
  dg5UNIT *Units[dg5_MAX_UNITS];
  INT NumOfUnits;

  /* Timer data */
  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per seond value */
  BOOL
    IsPause;
  /* KeyBoard */
  BYTE Keys[256];
  BYTE KeysOld[256];
  BYTE KeysClick[256];
  
  INT
    Mx, My, Mz,     /* Absolute coordinates */
    Mdx, Mdy, Mdz;  /* Relative coordinates shift */

   BYTE
    JBut[32],                    /* Joystick button state */
    JButOld[32],                 /* Joystick button old state */
    JButClick[32];               /* Joystick button click info */
  INT JPov;                      /* Joystick point-of-view control [0,1..8] */
  DBL
    Jx, Jy, Jz, Jr;              /* Joystick axes */


};

VOID DG5_AnimInit( HWND hWnd );
VOID DG5_AnimClose( VOID );
VOID DG5_AnimResize( INT W, INT H );
VOID DG5_AnimCopyFrame( HDC hDC );
VOID DG5_AnimRender( VOID );
VOID DG5_AnimAddUnit( dg5UNIT *Uni );
VOID DG5_AnimDoExit( VOID );
VOID DG5_AnimFlipFullScreen( VOID );

#endif