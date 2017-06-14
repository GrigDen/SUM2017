/* FILE NAME: T02CLOCK.C
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

               
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  BITMAP Bm;
  POINT pt;
  PAINTSTRUCT ps;
  INT x, y, i;
  static HDC hMemDC, hMemDCLogo;   
  static INT w, h;
  static HBITMAP hBm, hBmXOR, hBmAND;
  SYSTEMTIME st;
  FLOAT t, sec, min, hour;

  switch(Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC); 
    hBm = NULL;
    hBmAND = LoadImage(NULL, "AND.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmXOR = LoadImage(NULL, "XOR.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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
  case WM_LBUTTONDOWN:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_TIMER:
    GetObject(hBmXOR, sizeof(BITMAP), &Bm);
    SelectObject(hMemDCLogo, hBmAND);
    BitBlt(hDC, x, y, w - Bm.bmWidth, h - Bm.bmHeight, Bm.bmWidth, Bm.bmHeight, hMemDCLogo, SRCAND);
    SelectObject(hMemDCLogo, hBmXOR);
    BitBlt(hDC, x, y, w - Bm.bmWidth, h - Bm.bmHeight, Bm.bmWidth, Bm.bmHeight, hMemDCLogo, SRCINVERT);
    InvalidateRect(hWnd, NULL, FALSE);      
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    SelectObject(hMemDCLogo, hBmAND);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    SelectObject(hMemDCLogo, hBmXOR);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);

    EndPaint(hWnd, &ps);
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      DestroyWindow(hWnd);
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
}/*End of "MyWindowFunc" function*/


