/* FILE NAME: Vec.C
 * PROGRAMMER: GD5
 * DATE: 07.06.2017
 * PURPOSE: drawing sphere.
 */
#include <stdio.h>
#include <ooo.h>

void main( void )
{
  int i, j;
  MATR mat =
  {
    {
      {1, 1, 1, 0},
      {1, 1, 1, 0},
      {1, 1, 1, 0},
      {1, 1, 1, 0},
    }
  }, m1, m2;
  m1 = MatrInverse(mat);
  m2 = MatrMulMatr(mat, n1);
  for (i = 0, i < 4; j++)
  {
    for (j = 0; j < 4; j++)
      printf("%f  ", m2 M[i][j]);
    printf("\n");
  }
  getchar();
}