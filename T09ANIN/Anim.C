#include <string.h>

#include "anim.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm")

/* Joystic axis value determination */
#define DG5_GET_JOYSTICK_AXIS(A) \
  (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min - 1) - 1)

dg5ANIM DG5_Anim;
INT DG5_MouseWheel;

static UINT64 DG5_StartTime, DG5_OldTime, DG5_OldTimeFPS, DG5_PauseTime, DG5_TimePerSec, DG5_FrameCounter;

VOID DG5_AnimInit( HWND hWnd )
{
  HDC hDC;
  LARGE_INTEGER t;
  PIXELFORMATDESCRIPTOR pfd = {0};
  INT i;

  memset(&DG5_Anim, 0, sizeof(dg5ANIM));

  /* Store window and create memory device context */
  DG5_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  DG5_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);

  QueryPerformanceFrequency(&t);
  DG5_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  DG5_StartTime = DG5_OldTime = DG5_OldTimeFPS = t.QuadPart;
  DG5_PauseTime = 0;
  DG5_FrameCounter = 0;


  memset(&DG5_Anim, 0, sizeof(dg5ANIM));

  /* Store window and create memory device context */
  DG5_Anim.hWnd = hWnd;
  DG5_Anim.hDC = GetDC(hWnd);
  /* OpenGL initialization: setup pixel format */
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(DG5_Anim.hDC, &pfd);
  DescribePixelFormat(DG5_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(DG5_Anim.hDC, i, &pfd);

  /* OpenGL initialization: setup rendering context */
  DG5_Anim.hGLRC = wglCreateContext(DG5_Anim.hDC);
  wglMakeCurrent(DG5_Anim.hDC, DG5_Anim.hGLRC);
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(DG5_Anim.hGLRC);
    ReleaseDC(DG5_Anim.hWnd, DG5_Anim.hDC);
    return FALSE;
  }
  DG5_RndInit();
  return TRUE;
 
} /* End of 'DG5_AnimInit' function */

VOID DG5_AnimClose( VOID )
{
  INT i;

  /* Destroy all units */
  for (i = 0; i < DG5_Anim.NumOfUnits; i++)
  {
    DG5_Anim.Units[i]->Close(DG5_Anim.Units[i], &DG5_Anim);
    free(DG5_Anim.Units[i]);
  }
  DG5_Anim.NumOfUnits = 0;
  /* Delete OpenGL data */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(DG5_Anim.hGLRC);
  /* Delete GDI data */
  ReleaseDC(DG5_Anim.hWnd, DG5_Anim.hDC);

  memset(&DG5_Anim, 0, sizeof(dg5ANIM));
} /* End of 'DG5_AnimClose' function */

VOID DG5_AnimResize( INT W, INT H )
{
  /* Store new window frame size */
  DG5_Anim.W = W;
  DG5_Anim.H = H;

  glViewport(0, 0, W, H); 
  /* Create double buffer image */
  /*if (DG5_Anim.hFrame != NULL)
    DeleteObject(DG5_Anim.hFrame);
  hDC = GetDC(DG5_Anim.hWnd);
  DG5_Anim.hFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(DG5_Anim.hWnd, hDC);

  SelectObject(DG5_Anim.hDC, DG5_Anim.hFrame);
  DG5_RndSetProj();  */
} /* End of 'DG5_AnimResize' function */

VOID DG5_AnimCopyFrame( HDC hDC )
{
  SwapBuffers(DG5_Anim.hDC);
} /* End of 'DG5_AnimCopyFrame' function */


VOID DG5_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER t;
  POINT pt;

  /*** Send response to all units ***/
  for (i = 0; i < DG5_Anim.NumOfUnits; i++)
    DG5_Anim.Units[i]->Response(DG5_Anim.Units[i], &DG5_Anim);

  /*** Clear frame ***/
  /* Clear background */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  /*** Render all units ***/
  for (i = 0; i < DG5_Anim.NumOfUnits; i++)
  {
    SelectObject(DG5_Anim.hDC, GetStockObject(DC_PEN));
    SelectObject(DG5_Anim.hDC, GetStockObject(NULL_BRUSH));
    SetDCPenColor(DG5_Anim.hDC, RGB(0, 255, 0));
    SetDCBrushColor(DG5_Anim.hDC, RGB(255, 255, 255));

    DG5_Anim.Units[i]->Render(DG5_Anim.Units[i], &DG5_Anim);
  }

  /*** Handle timer ***/
  DG5_FrameCounter++;
  QueryPerformanceCounter(&t);
  /* Global time */
  DG5_Anim.GlobalTime = (DBL)(t.QuadPart - DG5_StartTime) / DG5_TimePerSec;
  DG5_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - DG5_OldTime) / DG5_TimePerSec;
  /* Time with pause */
  if (DG5_Anim.IsPause)
  {
    DG5_PauseTime += t.QuadPart - DG5_OldTime;
    DG5_Anim.DeltaTime = 0;
  }
  else
  {
    DG5_Anim.Time = (DBL)(t.QuadPart - DG5_PauseTime - DG5_StartTime) / DG5_TimePerSec;
    DG5_Anim.DeltaTime = DG5_Anim.GlobalDeltaTime;
  }
  /* FPS */
  if (t.QuadPart - DG5_OldTimeFPS > DG5_TimePerSec)
  {
    DG5_Anim.FPS = (DBL)DG5_FrameCounter * DG5_TimePerSec / (t.QuadPart - DG5_OldTimeFPS);
    DG5_OldTimeFPS = t.QuadPart;
    DG5_FrameCounter = 0;
  }
  DG5_OldTime = t.QuadPart;

  GetKeyboardState(DG5_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    DG5_Anim.Keys[i] >>= 7;
    DG5_Anim.KeysClick[i] = DG5_Anim.Keys[i] && !DG5_Anim.KeysOld[i];
  }
  memcpy(DG5_Anim.KeysOld, DG5_Anim.Keys, 256);

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
} /* End of 'DG5_AnimRender' function */

VOID DG5_AnimAddUnit( dg5UNIT *Uni )
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
  static BOOL IsExit = FALSE;

  if(IsExit)
    return;
  IsExit = TRUE;
  PostMessage(DG5_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'DG5_AnimAddUnit' function */

VOID DG5_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (IsFullScreen)
  {
    /* restore window size */
    SetWindowPos(DG5_Anim.hWnd, HWND_NOTOPMOST, SaveRect.left, SaveRect.top, SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top, SWP_NOOWNERZORDER);
  }
  else
  {
    /* Set full screen size to window */
    HMONITOR hmon;
    MONITORINFOEX moninfo;
    RECT rc;

    /* Store window old size */
    GetWindowRect(DG5_Anim.hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(DG5_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* Obtain monitor info */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* Set window new size */
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(DG5_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(DG5_Anim.hWnd, HWND_TOPMOST, rc.left, rc.top, rc.right - rc. left, rc.bottom - rc.top, SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'DG5_AnimFlipFullScreen' function */

/* END OF 'ANIM.C' FILE */