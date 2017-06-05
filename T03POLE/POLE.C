/* FILE NAME: T03EYES.C
 * PROGRAMMER: DG5
 * DATE: 05.06.2017
 * PURPOSE: Pole drawing program.
 */
/* Drowing ARROW*/
#include <windows.h>
#include <math.h>
#include "POLE.h"

/* Drowing EYE*/
VOID DrawArrow( HDC hDC, INT xc, INT yc, INT R, INT r, INT x, INT y, INT w, INT h, HWND hWnd)
{
  INT dx = x - xc, dy = y - yc, xm, ym;
  POINT pt, pts[4];  
  
  FLOAT l = sqrt(dx * dx + dy * dy), mysin = dy / l, mycos = dx / l;
  
  /*Geting Cursor position */
  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  
  /*Giving adres for points of arrow*/
  xm = x * mycos - y * mysin;
  ym = x * mysin + y * mycos; 
  pts[0].x = x * mycos - y * mysin;
  pts[0].y = ym;                   ד98נ89ננ
  pts[1].x = 5;
  pts[1].y = ym + 15;
  pts[2].x = (xm) ;
  pts[2].y = (ym + 20);
  pts[3].x = (xm - 5);
  pts[3].y = (ym + 15);
 
  
 Polygon(hDC, pts, 4);
}
  /*INT dx = x - xc, dy = y - yc, xm, ym;
  POINT pt;
  FLOAT l = sqrt(dx * dx + dy * dy), mysin = dy / l, mycos = dx / l;

  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  xm = xc + (R - r) * mycos;
  ym = yc + (R - r) * mysin;
  SelectObject(hDC, GetStockObject(WHITE_BRUSH));
  Ellipse(hDC, xc - 20, yc - 20, xc + 20, yc + 20);
  SelectObject(hDC, GetStockObject(BLACK_BRUSH));
  Ellipse(hDC, xm - 10, ym - 10, xm + 10, ym + 10);
  SelectObject(hDC,GetStockObject(NULL_BRUSH));*/
