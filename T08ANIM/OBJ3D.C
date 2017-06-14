/* FILE NAME: OBJ3D.C
 * PROGRAMMER: DG5
 * DATE: 10.06.2017
 * PURPOSE: Object handle module.
 */

#include <stdio.h>

#include "anim.h"
#include "render.h"

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       dg5OBJ3D *Obj;
 *   - model *.OBJ file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DG5_RndObjLoad( dg5OBJ3D *Obj, CHAR *FileName )
{
  INT vn = 0, fn = 0, size;
  FILE *F;
  static CHAR Buf[1000];

  memset(Obj, 0, sizeof(dg5OBJ3D));
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count quantities */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      fn++;
  }

  /* Memory allocation */
  size = sizeof(VEC) * vn + sizeof(INT [3]) * fn;
  if ((Obj->V = malloc(size)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Obj->F = (INT (*)[3])(Obj->V + vn);
  Obj->NumOfV = vn;
  Obj->NumOfF = fn;

  /* Read model data */
  rewind(F);
  vn = 0;
  fn = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL X, Y, Z;

      sscanf(Buf + 2, "%lf%lf%lf", &X, &Y, &Z);
      Obj->V[vn++] = VecSet(X, Y, Z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n0, n1, n2;

      if (sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &n0, &n1, &n2) == 3 ||
          sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &n0, &n1, &n2) == 3 ||
          sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &n0, &n1, &n2) == 3 ||
          sscanf(Buf + 2, "%i %i %i", &n0, &n1, &n2) == 3)
      {
        Obj->F[fn][0] = n0 - 1;
        Obj->F[fn][1] = n1 - 1;
        Obj->F[fn][2] = n2 - 1;
        fn++;
      }
    }
  }

  fclose(F);
  return TRUE;
} /* End of 'DG5_RndObjLoad' function */

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       dg5OBJ3D *Obj;
 * RETURNS: None.
 */
VOID DG5_RndObjFree( dg5OBJ3D *Obj )
{
  if (Obj->V != NULL)
    free(Obj->V);
  memset(Obj, 0, sizeof(dg5OBJ3D));
} /* End of 'DG5_RndObjFree function */

/* Object drawing function.
 * ARGUMENTS:
 *   - object pointer:
 *       dg5OBJ3D *Obj;
 *   - world coordinate system transform matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID DG5_RndObjDraw( dg5OBJ3D *Obj, MATR M )
{
  INT i;
  POINT *pts;
  MATR WVP;

  if ((pts = malloc(sizeof(POINT) * Obj->NumOfV)) == NULL)
    return;
  /* M = MatrMulMatr(M, DG5_RndMatrView); */

  WVP = MatrMulMatr(M, MatrMulMatr(DG5_RndMatrView, DG5_RndMatrProj));

  /* Project all points */
  for (i = 0; i < Obj->NumOfV; i++)
  {
    VEC p = VecMulMatr(Obj->V[i], WVP);
    /*xp = p.X * DG5_RndProjDist / -p.Z,
      yp = p.Y * DG5_RndProjDist / -p.Z; 
      pts[i].X = DG5_Anim.W / 2 + xp * DG5_Anim.W / DG5_RndWp;
      pts[i].Y = DG5_Anim.H / 2 - yp * DG5_Anim.H / DG5_RndHp;*/

    pts[i].x = (p.X + 1) * DG5_Anim.W / 2;
    pts[i].y = (-p.Y + 1) * DG5_Anim.H / 2;
  }

  /* Draw all facets */
  SelectObject(DG5_Anim.hDC, GetStockObject(NULL_BRUSH));
  SelectObject(DG5_Anim.hDC, GetStockObject(WHITE_PEN));
  for (i = 0; i < Obj->NumOfF; i++)
  {
    POINT p[3];

    p[0] = pts[Obj->F[i][0]];
    p[1] = pts[Obj->F[i][1]];
    p[2] = pts[Obj->F[i][2]];
    
    Polygon(DG5_Anim.hDC, p, 3);
  }


   /* SelectObject(DG5_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(DG5_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(DG5_Anim.hDC, RGB(0, 255, 255));
    SetDCPenColor(DG5_Anim.hDC, RGB(255, 0, 0));
    */
  /*for (i = 0; i < Obj->NumOfF; i++)
  {
    POINT *p = &pts[Obj->F[i][0]];
    MoveToEx(DG5_Anim.hDC, p->X, p->Y, NULL);

    p = &pts[Obj->F[i][1]];
    LineTo(DG5_Anim.hDC, p->X, p->Y);

    p = &pts[Obj->F[i][2]];
    LineTo(DG5_Anim.hDC, p->X, p->Y);

    p = &pts[Obj->F[i][0]];
    LineTo(DG5_Anim.hDC, p->X, p->Y);
  }                     */
  free(pts);
} /* End of 'DG5_RndObjDraw' function */

/* END OF 'OBJ3D.C' FILE */