/* FILE NAME: PRIM.C
 * PROGRAMMER: DG5
 * DATE: 14.06.2017
 * PURPOSE: Primitive handle module.
 */

#include <stdio.h>

#include "anim.h"

/* Create primitive function.
 * ARGUMENTS:
 *   - created primitive:
 *       dg5PRIM *Pr;
 *   - trimesh flag:
 *       BOOL IsTrimesh;
 *   - vertex array:
 *       dg5VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV
 *   - index array:
 *       INT *I;
 *   - index array size:
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID DG5_RndPrimCreate( dg5PRIM *Pr, BOOL IsTrimesh,
                        dg5VERTEX *V, INT NumOfV,
                        INT *I, INT NumOfI )
{
  memset(Pr, 0, sizeof(dg5PRIM));
  Pr->NumOfI = NumOfI;
  Pr->IsTrimesh = IsTrimesh;
  Pr->M = MatrIdentity();

  /* Generate buffers */
  glGenVertexArrays(1, &Pr->VA);
  glGenBuffers(1, &Pr->VBuf);

  /* Activate vertex array */
  glBindVertexArray(Pr->VA);
  /* Activate vertex buffer */
  glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
  /* Transfer vertex data */
  glBufferData(GL_ARRAY_BUFFER, sizeof(dg5VERTEX) * NumOfV, V, GL_STATIC_DRAW);

  /* Setup layout */
  /*                    layout (attribute number),
   *                       components count,
   *                          type,
   *                                   normalization flag,
   *                                       stride of buffer element,
   *                                                  offset to attribute from structure start */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(dg5VERTEX),
                        (VOID *)0); /* position */
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(dg5VERTEX),
                        (VOID *)sizeof(VEC)); /* texture coordinates */
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(dg5VERTEX),
                        (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normal */
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(dg5VERTEX),
                        (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color */

  /* Enable attributes */
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  /* Disable vertex array */
  glBindVertexArray(0);

  /* Generate index buffer */
  glGenBuffers(1, &Pr->IBuf);

  /* Activate index buffer */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  /* Transfer data */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
} /* Ens of 'DG5_RndPrimCreate' function */

/* Free primitive function.
 * ARGUMENTS:
 *   - deleted primitive:
 *       dg5PRIM *Pr;
 * RETURNS: None.
 */
VOID DG5_RndPrimFree( dg5PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);

  glDeleteBuffers(1, &Pr->IBuf);
} /* Ens of 'DG5_RndPrimFree' function */

/* Draw primitive function.
 * ARGUMENTS:
 *   - drawing primitive:
 *       dg5PRIM *Pr;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID DG5_RndPrimDraw( dg5PRIM *Pr, MATR M )
{
  INT loc, prg;
  MATR W, WVP;

  W = MatrMulMatr(Pr->M, M);
  WVP = MatrMulMatr3(W, DG5_RndMatrView, DG5_RndMatrProj);
  glLoadMatrixf(WVP.a[0]);

  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  prg = DG5_RndMaterialApply(Pr->MtlNo);
  /// glUseProgram(DG5_RndProgId);
  loc = glGetUniformLocation(prg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, WVP.a[0]);
  loc = glGetUniformLocation(prg, "MatrW");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, W.a[0]);
  loc = glGetUniformLocation(prg, "MatrV");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DG5_RndMatrView.a[0]);

  loc = glGetUniformLocation(prg, "LightPos");
  if (loc != -1)
    glUniform3fv(loc, 1, &DG5_RndLightPos.X);
  loc = glGetUniformLocation(prg, "LightColor");
  if (loc != -1)
    glUniform3fv(loc, 1, &DG5_RndLightColor.X);

  loc = glGetUniformLocation(prg, "Time");
  if (loc != -1)
    glUniform1f(loc, DG5_Anim.Time);
  loc = glGetUniformLocation(prg, "GTime");
  if (loc != -1)
    glUniform1f(loc, DG5_Anim.GlobalTime);

  glDrawElements(Pr->IsTrimesh ? GL_TRIANGLES : GL_TRIANGLE_STRIP,
    Pr->NumOfI, GL_UNSIGNED_INT, NULL);

  glBindVertexArray(0);
} /* Ens of 'DG5_RndPrimDraw' function */

/* Primitive load function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       dg5OBJ3D *Obj;
 *   - model *.OBJ file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DG5_RndPrimLoad( dg5PRIM *Obj, CHAR *FileName )
{
  INT vn = 0, fn = 0, size;
  FILE *F;
  dg5VERTEX *V;
  INT *I;
  static CHAR Buf[1000];

  memset(Obj, 0, sizeof(dg5PRIM));
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
  size = sizeof(dg5VERTEX) * vn + sizeof(INT) * fn * 3;
  if ((V = malloc(size)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  I = (INT *)(V + vn);
  memset(V, 0, size);

  /* Read model data */
  rewind(F);
  vn = 0;
  fn = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      V[vn++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n0, n1, n2;

      if (sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &n0, &n1, &n2) == 3 ||
          sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &n0, &n1, &n2) == 3 ||
          sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &n0, &n1, &n2) == 3 ||
          sscanf(Buf + 2, "%i %i %i", &n0, &n1, &n2) == 3)
      {
        I[fn++] = n0 - 1;
        I[fn++] = n1 - 1;
        I[fn++] = n2 - 1;
      }
    }
  }
  fclose(F);

  DG5_RndTriMeshEvalNormals(V, vn, I, fn);
  DG5_RndPrimCreate(Obj, TRUE, V, vn, I, fn);

  free(V);
  return TRUE;
} /* End of 'DG5_RndPrimLoad' function */

/* Evaluate trimesh vertex normals function.
 * ARGUMENTS:
 *   - vertex array:
 *       dg5VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV;
 *   - index array:
 *       INT *I;
 *   - index array size:
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID DG5_RndTriMeshEvalNormals( dg5VERTEX *V, INT NumOfV, INT *I, INT NumOfI )
{
  INT i;

  /* Set to zero all normals */
  for (i = 0; i < NumOfV; i++)
    V[i].N = VecSet1(0);

  /* Evaluate all facets normals */
  for (i = 0; i < NumOfI; i += 3)
  {
    dg5VERTEX *p0 = &V[I[i]], *p1 = &V[I[i + 1]], *p2 = &V[I[i + 2]];
    VEC N = VecNormalize(VecCrossVec(VecSubVec(p1->P, p0->P), VecSubVec(p2->P, p0->P)));

    /* Disperse normal to all three points */
    p0->N = VecAddVec(p0->N, N);
    p1->N = VecAddVec(p1->N, N);
    p2->N = VecAddVec(p2->N, N);
  }

  /* Normalize all normals */
  for (i = 0; i < NumOfV; i++)
    V[i].N = VecNormalize(V[i].N);
} /* End of 'DG5_RndTriMeshEvalNormals' function */

/* Evaluate grid vertex normals function.
 * ARGUMENTS:
 *   - vertex array:
 *       dg5VERTEX *V;
 *   - grid split numbers:
 *       INT N, M;
 * RETURNS: None.
 */
VOID DG5_RndGridEvalNormals( dg5VERTEX *V, INT N, INT M )
{
  INT i, j, k;

  /* Set to zero all normals */
  for (i = 0; i < N * M; i++)
    V[i].N = VecSet1(0);

  /* Evaluate all facets normals */
  for (k = 0, i = 0; i < N - 1; i++)
    for (j = 0; j < M - 1; j++, k++)
    {
      dg5VERTEX
        *p00 = &V[k],
        *p10 = &V[k + M],
        *p01 = &V[k + 1],
        *p11 = &V[k + M + 1];
      VEC
        N1 = VecNormalize(VecCrossVec(VecSubVec(p00->P, p10->P), VecSubVec(p11->P, p10->P))),
        N2 = VecNormalize(VecCrossVec(VecSubVec(p11->P, p01->P), VecSubVec(p00->P, p01->P)));

      /* Disperse normal to all three points */
      p10->N = VecAddVec(p10->N, N1);
      p00->N = VecAddVec(p00->N, N1);
      p11->N = VecAddVec(p11->N, N1);

      p00->N = VecAddVec(p00->N, N2);
      p01->N = VecAddVec(p01->N, N2);
      p11->N = VecAddVec(p11->N, N2);
    }


  /* Normalize all normals */
  for (i = 0; i < N * M; i++)
    V[i].N = VecNormalize(V[i].N);
} /* End of 'DG5_RndGridEvalNormals' function */

/* Sphere primitive creation function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       dg5PRIM *Pr;
 *   - sphere center position:
 *       VEC C;
 *   - sphere radius:
 *       FLT R;
 *   - sphere split numbers:
 *       INT N, M;
 * RETURNS: None.
 */
VOID DG5_RndPrimCreateSphere( dg5PRIM *Pr, VEC C, FLT R, INT N, INT M )
{
  dg5VERTEX *V, *p;
  INT
    i, j, k, v,
    *I,
    size = sizeof(dg5VERTEX) * N * M + sizeof(INT) * (M * 2 + 1) * (N - 1);
  DBL theta, phi;

  memset(Pr, 0, sizeof(Pr));
  if ((V = malloc(size)) == NULL)
    return;
  memset(V, 0, size);
  I = (INT *)(V + N * M);

  /* Setup vertices */
  for (p = V, theta = 0, i = 0; i < N; i++, theta += PI / (N - 1))
    for (phi = 0, j = 0; j < M; j++, phi += 2 * PI / (M - 1), p++)
    {
      p->N.X = sin(theta) * sin(phi);
      p->N.Y = cos(theta);
      p->N.Z = sin(theta) * cos(phi);
      p->P = VecAddVec(C, VecMulNum(p->N, R));
      p->C = Vec4Set(1, 1, 1, 1);
      p->T = Vec2Set(j / (M - 1.0), i / (N - 1.0));
    }

  /* Setup indices */
  for (k = 0, v = 0, i = 0; i < N - 1; i++)
  {
    for (j = 0; j < M; j++, v++)
    {
      I[k++] = v + M;
      I[k++] = v;
    }
    I[k++] = -1;
  }
  DG5_RndPrimCreate(Pr, FALSE, V, N * M, I, (M * 2 + 1) * (N - 1));

  free(V);
} /* Ens of 'DG5_RndPrimCreateSphere' function */

/* Plane primitive creation function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       dg5PRIM *Pr;
 *   - plane corner position:
 *       VEC C;
 *   - plane directions:
 *       VEC Du, Dv;
 *   - plane split numbers:
 *       INT N, M;
 * RETURNS: None.
 */
VOID DG5_RndPrimCreatePlane( dg5PRIM *Pr, VEC C, VEC Du, VEC Dv, INT N, INT M )
{
  dg5VERTEX *V, *p;
  INT
    i, j, k, v,
    *I,
    size = sizeof(dg5VERTEX) * N * M + sizeof(INT) * (M * 2 + 1) * (N - 1);
  VEC Norm = VecNormalize(VecCrossVec(Du, Dv));

  memset(Pr, 0, sizeof(Pr));
  if ((V = malloc(size)) == NULL)
    return;
  memset(V, 0, size);
  I = (INT *)(V + N * M);

  /* Setup vertices */
  for (p = V, i = 0; i < N; i++)
    for (j = 0; j < M; j++, p++)
    {
      p->N = Norm;
      p->P = VecAddVec(C,
        VecAddVec(VecMulNum(Du, j / (M - 1.0)), VecMulNum(Dv, i / (N - 1.0))));
      //p->P.Y += 2 * sin(j * 13.0) * cos(i * 13.0);
      p->C = Vec4Set(0.18, 0.30, 0.08, 1);
      p->T = Vec2Set(j / (M - 1.0), i / (N - 1.0));
    }

  /* Setup indices */
  for (k = 0, v = 0, i = 0; i < N - 1; i++)
  {
    for (j = 0; j < M; j++, v++)
    {
      I[k++] = v + M;
      I[k++] = v;
    }
    I[k++] = -1;
  }
  DG5_RndGridEvalNormals(V, N, M);
  DG5_RndPrimCreate(Pr, FALSE, V, N * M, I, (M * 2 + 1) * (N - 1));

  free(V);
} /* Ens of 'DG5_RndPrimCreatePlane' function */

/* END OF 'PRIM.C' FILE */
