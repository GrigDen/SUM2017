/* FILE NAME: VEC.H
 * PROGRAMMER: DG5
 * DATE: 09.06.17
 * PURPOSE: Library
 */

#ifndef VEC_H
#define VEC_H

#include <math.h>
#include <windows.h> 
                 
#pragma warning(disable: 4244)

#define PI 3.14159265358979323846
#define D2R(A) (A * PI / 180)
#define Degree2Radian(a) D2R(a)

typedef double DBL;
typedef float FLT;
typedef struct tagMATR
{
  FLT a[4][4];
} MATR;

typedef struct tagVEC
{
  FLT X, Y, Z;
} VEC;

typedef struct tagVEC2
{
  FLT X, Y;
} VEC2;

typedef struct tagVEC4
{
  FLT X, Y, Z, W;
} VEC4;

__inline FLT Rnd1()
{
  return sin(rand());    
}

__inline VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC v = {X, Y, Z};
  return v;
}

 __inline VEC2 Vec2Set( FLT X, FLT Y)
{
  VEC2 v = {X, Y};
  return v;
}

 __inline VEC4 Vec4Set( FLT X, FLT Y, FLT Z, FLT W )
{
  VEC4 v = {X, Y, Z, W};
  return v;
}                                  

__inline VEC VecSet1( FLT X )
{
  return VecSet(X, X, X);
}

__inline VEC VecAddVec( VEC v1, VEC v2 )
{
  return VecSet(v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z);
}

__inline VEC VecSubVec( VEC v1, VEC v2 )
{
  return VecSet(v1.X - v2.X, v1.Y - v2.Y, v1.Z - v2.Z);
}

__inline VEC VecMulNum( VEC v, FLT n )
{
  return VecSet(v.X * n, v.Y * n, v.Z * n);
}

__inline VEC VecDivNum( VEC v, FLT n )
{
  return VecSet(v.X / n, v.Y / n, v.Z / n);
}

__inline VEC VecNeg( VEC v )
{
  return VecSet(-v.X, -v.Y, -v.Z );
}

__inline FLT VecDotVec( VEC v1, VEC v2 )
{
  return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
}

static MATR UnitMatrix = 
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

__inline MATR MatrIdentity( VOID )
{
  return UnitMatrix;
}

__inline FLT VecLen2( VEC v )
{
  return v.X * v.X + v.Y * v.Y + v.Z * v.Z;
}

__inline FLT VecLen( VEC v )
{
  return sqrt(VecLen2(v));
}

__inline VEC VecNormalize( VEC v )
{
  FLT l = VecLen(v);

  return VecSet(v.X / l, v.Y / l, v.Z / l);
}

__inline MATR MatrMulMatr( MATR m1, MATR m2 )
{
  INT i, j, k;
  MATR r;

  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      for(r.a[i][j] = 0, k = 0; k < 4; k++)
        r.a[i][j] += m1.a[i][k] * m2.a[k][j];
  return r;
}
#define MatrMulMatr3(A, B, C)\
  (MatrMulMatr(A, MatrMulMatr(B, C)))
#define MatrMulMatr4(A, B, C, D)\
  (MatrMulMatr(A, MatrMulMatr3(B, C, D)))


__inline MATR MatrTranspose( MATR m )
{
  INT i, j;
  MATR r;

  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      r.a[i][j] = r.a[j][i];
  return r;
}

__inline FLT MatrDeterm3x3( FLT a11, FLT a12, FLT a13,
                            FLT a21, FLT a22, FLT a23,
                            FLT a31, FLT a32, FLT a33)
{
  return a11 * a22 * a33 - a11 * a23 * a32 - a12 * a21 * a33 + 
         a12 * a23 * a31 + a13 * a21 * a32 - a13 * a22 * a31;
}

__inline FLT MatrDeterm( MATR m )
{
  return
    m.a[0][0] * MatrDeterm3x3(m.a[1][1], m.a[1][2], m.a[1][3], 
                              m.a[2][1], m.a[2][2], m.a[2][3], 
                              m.a[3][1], m.a[3][2], m.a[3][3]) - 
    m.a[0][1] * MatrDeterm3x3(m.a[1][0], m.a[1][2], m.a[1][3], 
                              m.a[2][0], m.a[2][2], m.a[2][3], 
                              m.a[3][0], m.a[3][2], m.a[3][3]) +
    m.a[0][2] * MatrDeterm3x3(m.a[1][0], m.a[1][1], m.a[1][3], 
                              m.a[2][0], m.a[2][1], m.a[2][3], 
                              m.a[3][0], m.a[3][1], m.a[3][3]) - 
    m.a[0][3] * MatrDeterm3x3(m.a[1][0], m.a[1][1], m.a[1][1], 
                              m.a[2][0], m.a[2][1], m.a[2][1], 
                              m.a[3][0], m.a[3][1], m.a[3][1]);
}

__inline MATR MatrInverse( MATR m )
{
  INT i, j, p[][3] = {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}};
  FLT det = MatrDeterm(m);
  MATR r;

  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      r.a[i][j] = 
        MatrDeterm3x3(
          m.a[p[i][0]][p[j][0]], m.a[p[i][0]][p[j][1]], m.a[p[i][0]][p[j][2]], 
          m.a[p[i][1]][p[j][0]], m.a[p[i][1]][p[j][1]], m.a[p[i][1]][p[j][2]], 
          m.a[p[i][2]][p[j][0]], m.a[p[i][2]][p[j][1]], m.a[p[i][2]][p[j][2]]) / 
        det;
  return r;
}

/* function MatrRotate */
__inline MATR MatrRotate(FLT AngleDeg, VEC v )
{
  FLT a = D2R(AngleDeg), si = sin(a), co = cos(a);
  MATR m;
  VEC p = VecNormalize(v);
  
  m.a[0][0] = co + p.X * p.X * (1 - co);
  m.a[0][1] = p.Y * p.X * (1 - co) + p.Z * si;
  m.a[0][2] = p.Z * p.X * (1 - co) - p.Y * si;

  m.a[1][0] = p.X * p.Y * (1 - co) - p.Z * si;
  m.a[1][1] = co + p.Y * p.Y * (1 - co);
  m.a[1][2] = p.Z * p.Y * (1 - co) + p.X * si;

  m.a[2][0] = p.X * p.Z * (1 - co) + p.Y * si;
  m.a[2][1] = p.Y * p.Z * (1 - co) - p.X * si;
  m.a[2][2] = co + p.Z * p.Z * (1 -co);
  
  return m;
} /* End of MatrRotate */

__inline MATR MatrTranslate( VEC t )
{
  MATR m = UnitMatrix;

  m.a[3][0] = t.X;
  m.a[3][1] = t.Y;
  m.a[3][2] = t.Z;
  return m;
}

__inline MATR MatrScale( VEC s )
{
  MATR m = UnitMatrix;

  m.a[0][0] = s.X;
  m.a[1][1] = s.Y;
  m.a[2][2] = s.Z;
  return m;
}

__inline MATR MatrRotateZ( FLT AngleInDegree )
{
  MATR m = UnitMatrix;
  FLT a = D2R(AngleInDegree);

  m.a[0][0] = cos(a);
  m.a[0][1] = sin(a);
  m.a[1][0] = -sin(a);
  m.a[1][1] = cos(a);
  return m;
}

__inline MATR MatrRotateX( FLT AngleInDegree )
{
  MATR m = UnitMatrix;
  FLT a = D2R(AngleInDegree);

  m.a[1][1] = cos(a);
  m.a[1][2] = sin(a);
  m.a[2][1] = -sin(a);
  m.a[2][2] = cos(a);
  return m;
}

__inline MATR MatrRotateY( FLT AngleInDegree )
{
  MATR m = UnitMatrix;
  FLT a = D2R(AngleInDegree);

  m.a[0][0] = cos(a);
  m.a[0][2] = -sin(a);
  m.a[2][0] = sin(a);
  m.a[2][2] = cos(a);
  return m;
}

__inline VEC VecMulMatr( VEC v, MATR m )
{
  FLT w =v.X * m.a[0][3] + v.Y * m.a[1][3] + v.Z * m.a[2][3] + m.a [3][3];

  return VecSet((v.X * m.a[0][0] + v.Y * m.a[1][0] + v.Z * m.a[2][0] + m.a[3][0]) / w,
                (v.X * m.a[0][1] + v.Y * m.a[1][1] + v.Z * m.a[2][1] + m.a[3][1]) / w,
                (v.X * m.a[0][2] + v.Y * m.a[1][2] + v.Z * m.a[2][2] + m.a[3][2]) / w);
}

__inline VEC VecCrossVec( VEC v1, VEC v2 )
{
  return VecSet(v1.Y * v2.Z - v1.Z * v2.Y, v1.Z * v2.X - v1.X * v2.Z, v1.X * v2.Y - v1.Y * v2.X);
}

/* function MatrView */
__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {               Right.X,                Up.X,              -Dir.X, 0},
      {               Right.Y,                Up.Y,              -Dir.Y, 0},
      {               Right.Z,                Up.Z,              -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

/* function VecMulMatr43 */
__inline VEC VecMulMatr43( VEC V, MATR M )
{
  return VecSet(V.X * M.a[0][0] + V.Y * M.a[1][0] + V.Z * M.a[2][0] + M.a[3][0],
                V.X * M.a[0][1] + V.Y * M.a[1][1] + V.Z * M.a[2][1] + M.a[3][1],
                V.X * M.a[0][2] + V.Y * M.a[1][2] + V.Z * M.a[2][2] + M.a[3][2]);
} /* End of 'VecMulMatr43' function */

/* function VecMulMatr3 */
__inline VEC VecMulMatr3( VEC V, MATR M )
{
  return VecSet(V.X * M.a[0][0] + V.Y * M.a[1][0] + V.Z * M.a[2][0],
                V.X * M.a[0][1] + V.Y * M.a[1][1] + V.Z * M.a[2][1],
                V.X * M.a[0][2] + V.Y * M.a[1][2] + V.Z * M.a[2][2]);
} /* End of 'VecMulMatr3' function */

/* function PointTransform */
__inline VEC PointTransform( VEC V, MATR M )
{
  FLT w = V.X * M.a[0][3] + V.Y * M.a[1][3] + V.Z * M.a[2][3] + M.a[3][3];

  return VecSet((V.X * M.a[0][0] + V.Y * M.a[1][0] + V.Z * M.a[2][0] + M.a[3][0]) / w,
                (V.X * M.a[0][1] + V.Y * M.a[1][1] + V.Z * M.a[2][1] + M.a[3][1]) / w,
                (V.X * M.a[0][2] + V.Y * M.a[1][2] + V.Z * M.a[2][2] + M.a[3][2]) / w);
} /* End of 'PointTransform' function */

__inline MATR MatrFrustum( FLT l, FLT r, FLT b, FLT t, FLT n ,FLT f )
{
  MATR m = MatrIdentity();
  m.a[0][0] = 2 * n / (r - l);
  m.a[1][1] = 2 * n / (t - b);
  m.a[2][0] = (r + l) / (r - l);
  m.a[2][1] = (t + b) / (t - b);
  m.a[2][2] = -(f + n) / (f - n);
  m.a[2][3] = -1;
  m.a[3][2] = -2 * n * f / (f - n);
  m.a[3][3] = 0;
  return m;
} /* End of 'MatrFrustum' function */

#endif
/* EOF 'VEC.H' */