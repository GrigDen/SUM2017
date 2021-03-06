 /* FILE NAME: T01EYES.C
 * PROGRAMMER: DG5
 * DATE: 01.06.2017
 * PURPOSE: Eyes drawing program.
 */

#include <windows.h>
#include <math.h>

#pragma warning (disable: 4244)

#define WND_CLASS_NAME "My second window"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, 
                               WPARAM wParam,LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd)
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBoxA(NULL, "Error register window class", "Error", MB_OK);
    return 0;
  }

  hWnd =
    CreateWindow(WND_CLASS_NAME,
      "Title",
      WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  { 
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}/*End of "WinMain" function */

 
/* Drowing EYE*/
VOID DrawEye( HDC hDC, INT xc, INT yc, INT R, INT r, INT x, INT y, INT w, INT h, HWND hWnd)
{
  INT dx = x - xc, dy = y - yc, xm, ym, size;
  POINT pt;
  FLOAT l = sqrt(dx * dx + dy * dy), mysin = dy / l, mycos = dx / l;

  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  size = rand() % 2;
  xm = xc + (R - r) * mycos;
  ym = yc + (R - r) * mysin;
  SelectObject(hDC, GetStockObject(WHITE_BRUSH));
  Ellipse(hDC, xc - 20 * size, yc - 20 * size, xc + 20 * size, yc + 20 * size);
  SelectObject(hDC, GetStockObject(BLACK_BRUSH));
  Ellipse(hDC, xm - 10 * size, ym - 10 * size, xm + 10 * size, ym + 10 * size);
  SelectObject(hDC,GetStockObject(NULL_BRUSH));
  
}


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;

  INT x, y, i;
  POINT pt;
  PAINTSTRUCT ps;
  static HDC hMemDC;   
  static INT w, h;
  static HBITMAP hBm;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;
    SetTimer(hWnd, 47, 30, NULL);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 47, 0);
    return 0;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      DestroyWindow(hWnd);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;


  case WM_TIMER:
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    /*Drowing BackGroung */
    SelectObject(hMemDC, GetStockObject(WHITE_BRUSH));
    Rectangle(hMemDC, -1, -1, w + 1, h + 1);
    InvalidateRect(hWnd, NULL, FALSE);
    SelectObject(hMemDC, GetStockObject(NULL_BRUSH));

    Rectangle(hMemDC, -1, -1, w + 1, h + 1);
    srand(30);
    /*Drowing Objects */
    for (i = 0; i < 5000; i++)
    {
      x = rand() % w;
      y = rand() % h;
      DrawEye(hMemDC, x, y, 30, 20, pt.x, pt.y, w, h, hWnd); 

    }
    InvalidateRect(hWnd, NULL, FALSE);

    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' fuction */



  
