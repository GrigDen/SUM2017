/* FILE NAME: SPHERE.C
 * PROGRAMMER: GD5
 * DATE: 07.06.2017
 * PURPOSE: drawing sphere.
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sphere.h"

#pragma warning (disable: 4244)

#define GRID_H 50
#define GRID_W 35

static VEC G[GRID_H][GRID_W];
static POINT Pts[GRID_H][GRID_W];

MATR MatrRotate( DBL AngleInDegree, VEC R )
{
  DBL a = AngleInDegree * PI / 180, sine = sin(a), cosine = cos(a);
  MATR m;

  a = sqrt(R.x * R.x + R.y * R.y + R.z * R.z);
  R.x /= a;
  R.y /= a;
  R.z /= a;

  m.A[0][0] = cosine + R.x * R.x * (1 - cosine);
  m.A[0][1] = R.x * R.y * (1 - cosine) - R.z * sine;
  m.A[0][2] = R.x * R.z * (1 - cosine) + R.y * sine;

  m.A[1][0] = R.y * R.x * (1 - cosine) + R.z * sine;
  m.A[1][1] = cosine + R.y * R.y * (1 - cosine);
  m.A[1][2] = R.y * R.z * (1 - cosine) - R.x * sine;

  m.A[2][0] = R.z * R.x * (1 - cosine) - R.y * sine;
  m.A[2][1] = R.z * R.y * (1 - cosine) + R.x * sine;
  m.A[2][2] = cosine + R.z * R.z * (1 - cosine);
  return m;
} /* End of 'MatrRotate' function */

/* Vector transform function */
VEC VecTransform( VEC V, MATR m )
{
  
  VEC P =
  {
    V.x * m.A[0][0] + V.y * m.A[0][1] + V.z * m.A[0][2],
    V.x * m.A[1][0] + V.y * m.A[1][1] + V.z * m.A[1][2],
    V.x * m.A[2][0] + V.y * m.A[2][1] + V.z * m.A[2][2]
  };

  return P;
} /* End of 'VecTransform' function */

VOID BuildSphere( DBL R )
{
  DBL theta, phi, x, y, z;
  INT i, j;

  for (theta = 0, i = 0; i < GRID_H; i++, theta += PI / (GRID_H - 1))
    for (phi = 0, j = 0; j < GRID_W; j++, phi += 2 * PI / (GRID_W - 1))
    {
      x = R * sin(theta) * sin(phi);
      y = R * cos(theta);
      z = R * sin(theta) * cos(phi);

      G[i][j].x = R * sin(theta) * sin(phi);
      G[i][j].y = R * cos(theta);
      G[i][j].z = R * sin(theta) * cos(phi);
    } /* End of 'BuildSphere' function */
}

VOID DrawSphere( HDC hDC, INT w, INT h )
{
  DBL size = 1, wp = size, hp = size, xs, ys, t = clock() / (DBL)CLOCKS_PER_SEC , xp, yp;
  INT i, j;
  VEC v = {1, 2, 3};
  MATR m;

  m = MatrRotate ( t * 50, v);

  
  if (w > h)
    wp *= (DBL)w / h;
  else
    hp *= (DBL)h / w;

  m = MatrRotate ( t * 50, v);
  /* Matrix rotate*/
  SelectObject(hDC, GetStockObject(WHITE_BRUSH));
  SelectObject(hDC, GetStockObject(WHITE_PEN));
  for (i = 0; i < GRID_H - 1; i++)
    for (j = 0; j < GRID_W - 1; j++)
    {
      VEC p = VecTransform(G[i][j], m);
      xp = p.x;
      yp = p.y;

      Pts[i][j].x = xp * w / wp + w / 2;
      Pts[i][j].y = h / 2 - yp * h / hp;
      Ellipse(hDC, Pts[i][j].x - 3, Pts[i][j].y - 3, Pts[i][j].x + 3, Pts[i][j].y + 3);
    } 

    for (i = 0; i < GRID_H; i++)
      for (j = 0; j < GRID_W; j++)
      {
        POINT p[4];

        p[0] = Pts[i][j];
        p[1] = Pts[i + 1][j];
        p[2] = Pts[i + 1][j + 1];
        p[3] = Pts[i][j + 1];

        Polygon(hDC, p, 4);
      }
 /* for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
    {
      
      xp = G[i][j].x * cos(t) - G[i][j].y * sin(t);
      yp = G[i][j].x * sin(t) + G[i][j].y * cos(t);

      xs = xp * w / wp + w / 2;
      ys = h / 2 - yp * h / hp;
      Ellipse(hDC, xs - 3, ys - 3, xs + 3, ys + 3);
    } */
  SelectObject(hDC, GetStockObject(NULL_BRUSH));
  SelectObject(hDC, GetStockObject(NULL_PEN));
}/* End of 'DrawSphere'function */

/* END OF 'SPHERE.C' FILE */