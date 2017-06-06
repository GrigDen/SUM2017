/* FILE NAME: T05PERM.C
 * PROGRAMMER: DG5
 * DATE: 06.06.2017
 * PURPOSE: Deturmination of matrix.
 */

#include <stdio.h>
#include <math.h>
#define Max 15
 /* kik warning */
#pragma warning (disable: 4244)

int p[Max], parity, n, sign = 1;
double a[Max][Max], Det = 566, MAX_i, MAX_j, MAX;

void CountMAX( void )
{
  int i, j;
  MAX = a[0][0];
  for (i = 0; i < n; i++)
    for (j = 0; j < n; i++)
      if (MAX > a[i][j])
      {  
        MAX = fabs(a[i][j]);
        MAX_i = i;
        MAX_j = j;
      }
} 

void Swap( int *A, int *B )
{
  int tmp = *A;

  *A = *B;
  *B = tmp;
}
void MatrixSwapStrech( double MAX_i, double MAX_j )
{
  double tmp;
  int g, i, j;
  
   for (i = MAX_i, j = MAX_j, g = 1; i < n; i++)
   {
     tmp = a[i][j];
     a[i][j] = a[g][j];
     a[g][j] = a[i][j];
   }
}
void MatrixSwapTowers( double MAX_i, double MAX_j )
{
  double tmp;
  int g, i, j;

   for (j = MAX_j, i = MAX_i, g = 0; j < n; j++)
   {
     tmp = a[i][j];
     a[i][j] = a[i][g];
     a[i][g] = a[i][j];
   }
}
void Gaus( void )
{
  int i, j;

  for (i = 1; i < n; i++)
    for (j = 1; j < n; i++)
      a[i][j] = a[i][j] - a[0][0] * a[i][j] / a[0][0];
  /*counting Determination */
  for (i = 0, j = 0; i < n; i++, j++)
    Det *= a[i][j];
  Det *= sign;
  
}

void MatrixLoad( char* FileName )
{
 int i, j;
 FILE *f;

 if ((f = fopen("a.txt", "r")) == NULL)
   return;
 fscanf( f, "%d", &n);
 if (n > MAX)
   n = MAX;
 for (i = 0; i < n; i++)
   for(j = 0; j < n; j++)
     fscanf(f,"%lf", &a[i][j]);
 fclose(f);
}

void PrintDet( void )
 {
   FILE *f;
 
   f = fopen("OUTPUT.TXT", "a");
   fprintf(f, "NEWDet = %lf \n", Det);
   fclose(f);
 }
void Go( int pos )
{
  int i, Saveparity, tmp;
  double  mtmp = 1;
                                          
  if (pos >= n)
  {
    for (i = 0; i < n; i++)
      mtmp *= a[i][p[i]];
    if (parity)
      Det -= mtmp;
    else
      Det += mtmp;
    return;
  }
  Go(pos + 1);
  Saveparity = parity;
  for (i = pos + 1; i < n; i++)
  {
    Swap(&p[pos], &p[i]);
    parity = !parity;
    Go(pos + 1);
  }
  tmp = p[pos];
  for (i  = pos + 1; i < n; i++)
    p[i - 1] = p[i];
  p[n - 1] = tmp;
  parity = Saveparity;
}


void main( void )
{
  MatrixLoad("A.txt");
  CountMAX();
  if (MAX_i != 0)
  {
    MatrixSwapStrech( MAX_i, MAX_j );
    sign = -sign;
  }
   if (MAX_j != 0)
   {
     MatrixSwapTowers( MAX_i, MAX_j );
     sign = -sign;
   }
   /* Go(0);*/
  Gaus();
  PrintDet();
}







 




