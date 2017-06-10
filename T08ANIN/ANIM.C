/* FILE NAME: Vec.C
 * PROGRAMMER: GD5
 * DATE: 07.06.2017
 * PURPOSE: drawing sphere.
 */

#include "Anim.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm")


/* Timer local data */
static UINT64
  DG5_StartTime,  /* Start program time */
  DG5_OldTime,    /* Time from program start to previous frame */
  DG5_OldTimeFPS, /* Old time FPS measurement */
  DG5_PauseTime,  /* Time during pause period */
  DG5_TimePerSec, /* Timer resolution */
  DG5_FrameCounter; /* Frames counter */

DG5ANIM DG5_Anim;

VOID DG5_AnimInit( HWND hWnd )
{
  HDC hDC;
  LARGE_INTEGER t;

  /* Timer initialization */
  QueryPerformanceFrequency(&t);
  DG5_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  DG5_StartTime = DG5_OldTime = DG5_OldTimeFPS = t.QuadPart;
  DG5_PauseTime = 0;
  DG5_FrameCounter = 0;

  DG5_Anim.IsPause = FALSE;
  DG5_Anim.FPS = 50;

  memset(&DG5_Anim, 0, sizeof(DG5ANIM));
  DG5_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  DG5_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);

}

VOID DG5_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < DG5_Anim.NumOfUnits; i++)
  {
    DG5_Anim.Units[i]->Close(DG5_Anim.Units[i], &DG5_Anim);
    free(DG5_Anim.Units[i]);
  }
  DG5_Anim.NumOfUnits = 0;

  DeleteDC(DG5_Anim.hDC);
  DeleteObject(DG5_Anim.hFrame);

  memset(&DG5_Anim, 0, sizeof(DG5ANIM));

}

VOID DG5_AnimResize( INT w, INT h )
{
  HDC hDC;

  DG5_Anim.W = w;
  DG5_Anim.H = h;

  if (DG5_Anim.hFrame != NULL)
    DeleteObject(DG5_Anim.hFrame);
  hDC = GetDC(DG5_Anim.hWnd);
  DG5_Anim.hFrame = CreateCompatibleBitmap(hDC, w, h);
  ReleaseDC(DG5_Anim.hWnd, hDC);

  SelectObject(DG5_Anim.hDC, DG5_Anim.hFrame);
}

VOID DG5_AnimCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, DG5_Anim.W, DG5_Anim.H, DG5_Anim.hDC, 0, 0, SRCCOPY);
}

/* !!!!!!!!!RENDERING!!!!!!!!!!! */
VOID DG5_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER t;


  /*** Handle timer ***/
  DG5_FrameCounter++;                   /* -- increment frame counter (for FPS)*/
  QueryPerformanceCounter(&t);          /* -- obtain current timer value*/
  /* Global time */
  DG5_Anim.GlobalTime = (DBL)(t.QuadPart Ц DG5_StartTime) / DG5_TimePerSec;
  DG5_Anim.GlobalDeltaTime = (DBL)(t.QuadPart Ц DG5_OldTime) / DG5_TimePerSec;
  /* Time with pause */
  if (DG5_Anim.IsPause)
  {
    DG5_PauseTime += t.QuadPart Ц DG5_OldTime;
    DG5_Anim.DeltaTime = 0;
  }
  else
  {
    DG5_Anim.Time = (DBL)(t.QuadPart Ц DG5_PauseTime - DG5_StartTime) / DG5_TimePerSec;
    DG5_Anim.DeltaTime = DG5_Anim.GlobalDeltaTime;
  }
  /* FPS */
  if (t.QuadPart Ц DG5_OldTimeFPS > DG5_TimePerSec)
  {
    DG5_Anim.FPS = (DBL)DG5_FrameCounter * DG5_TimePerSec / (t.QuadPart Ц DG5_OldTimeFPS);
    DG5_OldTimeFPS = t.QuadPart;
    DG5_FrameCounter = 0;
  }
  DG5_OldTime = t.QuadPart;

  for (i = 0; i < DG5_Anim.NumOfUnits; i++)
    DG5_Anim.Units[i]->Response(DG5_Anim.Units[i], &DG5_Anim);
  SelectObject(DG5_Anim.hDC, GetStockObject(NULL_PEN));
  SelectObject(DG5_Anim.hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(DG5_Anim.hDC, RGB(0, 255, 255));
  Rectangle(DG5_Anim.hDC, 0, 0, DG5_Anim.W + 1, DG5_Anim.H + 1);

  for (i = 0; i < DG5_Anim.NumOfUnits; i++)
  {
    SelectObject(DG5_Anim.hDC, GetStockObject(NULL_BRUSH));
    SelectObject(DG5_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(DG5_Anim.hDC, RGB(0, 255, 255));
    Rectangle(DG5_Anim.hDC, 0, 0, DG5_Anim.W + 1, DG5_Anim.H + 1);

    DG5_Anim.Units[i]->Render(DG5_Anim.Units[i], &DG5_Anim);

  }
  GetKeyboardState(DG5_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    DG5_Anim.Keys[i] >>= 7;
    DG5_Anim.KeysClick[i] = DG5_Anim.Keys[i] && !DG5_Anim.KeysOld[i];<- ќтслеживание однократного нажати€
  }
  memcpy(DG5_Anim.KeysOld, DG5_Anim.Keys, 256);

  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(DG5_Anim.hWnd, &pt);
  DG5_Anim.Mdx = pt.x - DG5_Anim.Mx;
  DG5_Anim.Mdy = pt.y - DG5_Anim.My;
  DG5_Anim.Mx = pt.x;
  DG5_Anim.My = pt.y;

  DG5_Anim.Mdz = DG5_MouseWheel;
  DG5_Anim.Mz += DG5_MouseWheel;
  DG5_MouseWheel = 0;

  /*** Joystick handle ***/
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(JOYCAPS)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Axes */
        DG5_Anim.Jx = DG5_GET_JOYSTICK_AXIS(X);
        DG5_Anim.Jy = DG5_GET_JOYSTICK_AXIS(Y);
        DG5_Anim.Jz = DG5_GET_JOYSTICK_AXIS(Z);
        DG5_Anim.Jr = DG5_GET_JOYSTICK_AXIS(R);

        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          DG5_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
          DG5_Anim.JButClick[i] = DG5_Anim.JBut[i] && !DG5_Anim.JButOld[i];
          memcpy(DG5_Anim.JButOld, DG5_Anim.JBut, 32);
        }

        /* Point of view */
        DG5_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;
      }
    }
  }
} /* End of RENDERING */

VOID DG5_AnimAddUnit( DG5UNIT *Uni )
{
  if (DG5_Anim.NumOfUnits < DG5_MAX_UNITS)
  {
    DG5_Anim.Units[DG5_Anim.NumOfUnits] = Uni;
    DG5_Anim.Units[DG5_Anim.NumOfUnits]->Init(DG5_Anim.Units[DG5_Anim.NumOfUnits], &DG5_Anim);
    DG5_Anim.NumOfUnits++;
  }
}

VOID DG5_AnimDoExit( VOID )
{
  static BOOL isExit = FALSE;

  if (isExit)
    return;
  isExit = TRUE;
  PostMessage(DG5_Anim.hWnd, WM_CLOSE, 0, 0);
}

VOID DG5_AnimFlipFullScreen( VOID )
{
  static BOOL isFullScreen;
  static RECT SaveRect;
  if (isFullScreen)
  {
    SetWindowPos(DG5_Anim.hWnd, HWND_NOTOPMOST, 
      SaveRect.left, SaveRect.top, 
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top, 
      SWP_NOOWNERZORDER);
  }
  else
  {
    HMONITOR hMon;
    MONITORINFOEX moninfo;
    RECT rc;

    GetWindowRect(DG5_Anim.hWnd, &SaveRect);

    hMon = MonitorFromWindow(DG5_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hMon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(DG5_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(DG5_Anim.hWnd, HWND_NOTOPMOST, 
      rc.left, rc.top, 
      rc.right - rc.left, rc.bottom - rc.top, 
      SWP_NOOWNERZORDER);
  }
  isFullScreen = !isFullScreen;
}
/* End of 'ANIM.C' */
