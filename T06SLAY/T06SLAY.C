/* FILE NAME: T05SLAY.C
 * PROGRAMMER: DG5
 * DATE: 06.06.2017
 * PURPOSE: Deturmination of matrix.
 */

#include <stdio.h>
#include <math.h>
#define Max 15

#pragma warning (disable: 4244)

int p[Max], parity, n, sign = 1;
double a[Max][Max], Det = 1;

void Swap( double *A, double *B )
{
  double tmp = *A;

  *A = *B;
  *B = tmp;
}/* End of "Swap" function */

void Gauss( void )
{
  int i, j, k, x, y;
  double coef;

  for (i = 0; i < n; i++)
  { 
    /* looking for maximum matrix element in every row and colum */
    k = i;
    j = k;
    for (y = i; y < n; y++)
      for(x = i; x < n; x++)
        if (fabs(a[y][x]) > fabs(a[j][k]))
        {  
          k = x;
          j = y;
        }
     if (a[j][k] == 0)
     { 
       Det = 0;
       return;
     }
     if (j != i) /* Swap j row and i row (elements[i..n-1] */
     {
       for (x = i; x < n; x++)
         Swap( &a[i][x], &a[j][x]);
       Det = - Det;
     }
       
     if (k != i) /* Swap k colum and i colum (elements[i..n-1] */
     {
       for (x = i; x < n; x++)
         Swap(&a[x][i], &a[x][k]);
       Det = - Det;
     }
     /* Subtrack from every row [i+1...n-1] */
     for (j = i + 1; j < n; j++)
     {
       coef = a[j][i] / a[i][i];
       for (k = i; k < n; k++)
         a[j][k] -= a[i][k] * coef;
     }
  }/* End of FOR (i = 0; i < n; i++) */
  
  /* Counting determination */
  for (i = 0; i < n; i++)
    Det *= a[i][i];
}/* End of "Gauss" function */

void MatrixLoad( char* FileName )
{
 int i, j;
 FILE *f;

 if ((f = fopen("Matrix.txt", "r")) == NULL)
   return;
 fscanf(f, "%d", &n);
 if (n > Max)
   n = Max;
 for (i = 0; i < n; i++)
   for(j = 0; j < n; j++)
     fscanf(f,"%lf", &a[i][j]);
 fclose(f);
}/* End of "MatrixLoad" function */

void PrintDet( void )
 {
   FILE *f;
 
   f = fopen("Answer.TXT", "a");
   fprintf(f, "Det = %lf \n", Det);
   fclose(f);
 }/* End of "PrintDet" function */

void main( void )
{
  MatrixLoad("Matrix.txt");
  Gauss();
  PrintDet();
} /* End of "main" function */