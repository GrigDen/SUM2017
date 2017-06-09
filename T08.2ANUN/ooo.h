/* FILE NAME: Vec.C
 * PROGRAMMER: GD5
 * DATE: 07.06.2017
 * PURPOSE: drawing sphere.
 */

#include <math.h>

#define PI 3.14159265358979323846
#define D2R(A) ((A) * PI / 180))
#define Degree@Radian (a) D2R(a)

typedef double DBL;

typedef struct tagVEC
{
  DBL M[4][4];
}  MATR;

VEC VecSet( DBL x, DBL y, DBL z )
{
  VEC v;
  v.x = x;
  v.y = y;
  v.z = z;
  retutn v;
} /* End of "VecSet" function */

static MATR UnitMatrix =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
}

__inline Matr MatrIdentiti( VOID )
{
  return UintMatrix;

  m.A[3][0] = T.x;
  m.A[3][1] = T.y;
  m.A[3][2] = T.z;
  return m;
}

MATR MatrScale( VEC S );
MATR MatrRotateX( DBL AngleInDegree );
MATR MatrRotateY( DBL AngleInDegree );
MATR MatrRotateZ( DBL AngleInDegree );

VEC VecAddVec( VEC V1, VEC V2 );
VEC VecAddVec( VEC V1, VEC V2 );
VEC VecAddVec( VEC V1, DBL N );
VEC VecAddVec( VEC V1, DBL N );
VEC VecNeg( VEC V );

/* dotproduction of  vec*/
DBL VecDotVec( Vec V1, VEC V2 );
/* poduction of vec */
VEC VecCrossVec( VEC V1, VEC V2 );

DBL VecLen( VEC V );
DBL VecLen2( VEC V );
DBL VecNormolize( VEC V );

VEC VecMulMatr( VEC V, MATR M );

__inline VEC VecAddVecEq( VEC *VRes, VEC V2 );
{
  VRes.x += V2.x;
  VRes.y += V2.y;
  VRes.z += V2.z;
  
  return *VRes;
}

VEC PointTransform( VEC V, MATR M);
VECVectorTransform( VEC V, MATR M);

/* SHAPING*/
MATR Q = MatrTransform( VEC V, MATR M);
N1 = VectorTransform(N, Q);

__inline MATR MatrRotate ( DBL AngleInDegree, VEC R )
{
  DBL A = D2R (AngleToDegree), si = sin(A), co = cos(A);
  VEC V = VecNormalize(R);
  MATR M = 
  {
    {
     {co + V.x * V.x * (1 - co), V.x * V.y * (1 - co) - V.z * si, V.x * V.z * (1 - co) + v.Y + si, 0},
     {V.y * V.x * (1 - co) + V.z * si, co + V.y * V.y * (1 - co), V.y * V.z * (1 - co) - V.x * si, 0}, 
     {V.z * V.x * (1 - co) - V.y * si, V.z * V.y * (1 - co) + V.x * si, co + V.z * V.z * (1 -co), 0},
     {0, 0, 0, 1}
    }
   };
   
   return M;
}

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r; 
   
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (r.A[i][j] = 0, k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][j] * M2.A[k][j];
  return r;
}

MATR MatrTranspose( MATR M );

DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A23, DBL A33 )
{ 
  return A11 * A22 * A33 - A11 * A23 * A32 - A12 * A21 * A33 + A12 * A23 * A31 + A13 * A21 * A32 - A13 * A22 * A31;
}

DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3],) -
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3],) + 
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3],) -
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3],);
}

MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);

  INT p[4][3] = {{1, 2, 3}, {0, 2, 3}, {0, 1, 2}};
  
  if (det == 0)
    return UnitMatrix;
  for (i == 0; i < 4; i++)
    for (j == 0; j < 4; j++)
      r.A[j][i] = 
      MatrDeterm#x3(
        M.A[p[i][0]][i[j][0]], M.A[p[i][0]][i[j][1]], M.A[p[i][0]][i[j][2]],
        M.A[p[i][1]][i[j][0]], M.A[p[i][1]][i[j][1]], M.A[p[i][1]][i[j][2]],
        M.A[p[i][2]][i[j][0]], M.A[p[i][2]][i[j][1]], M.A[p[i][2]][i[j][2]]) / det;
  return r;
}

/* Matrix look-at viewer setup function.
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       VEC Loc, At, Up1;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {               Right.x,                Up.x,              -Dir.x, 0},
      {               Right.y,                Up.y,              -Dir.y, 0},
      {               Right.z,                Up.z,              -Dir.z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

/* Vector by matrix multiplication (without projection) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr43( VEC V, MATR M )
{
  return VecSet(V.x * M.A[0][0] + V.y * M.A[1][0] + V.z * M.A[2][0] + M.A[3][0],
                V.x * M.A[0][1] + V.y * M.A[1][1] + V.z * M.A[2][1] + M.A[3][1],
                V.x * M.A[0][2] + V.y * M.A[1][2] + V.z * M.A[2][2] + M.A[3][2]);
} /* End of 'VecMulMatr43' function */

/* Vector by matrix multiplication (only orientation) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr3( VEC V, MATR M )
{
  return VecSet(V.x * M.A[0][0] + V.y * M.A[1][0] + V.z * M.A[2][0],
                V.x * M.A[0][1] + V.y * M.A[1][1] + V.z * M.A[2][1],
                V.x * M.A[0][2] + V.y * M.A[1][2] + V.z * M.A[2][2]);
} /* End of 'VecMulMatr3' function */

/* Vector by matrix multiplication (with homogenious devide) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC PointTransform( VEC V, MATR M )
{
  FLT w = V.x * M.A[0][3] + V.y * M.A[1][3] + V.z * M.A[2][3] + M.A[3][3];

  return VecSet((V.x * M.A[0][0] + V.y * M.A[1][0] + V.z * M.A[2][0] + M.A[3][0]) / w,
                (V.x * M.A[0][1] + V.y * M.A[1][1] + V.z * M.A[2][1] + M.A[3][1]) / w,
                (V.x * M.A[0][2] + V.y * M.A[1][2] + V.z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'PointTransform' function */