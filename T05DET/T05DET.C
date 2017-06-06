/* FILE NAME: T05PERM.C
 * PROGRAMMER: DG5
 * DATE: 06.06.2017
 * PURPOSE: Deturmination of matrix.
 */

#include <stdio.h>
#define max 15

#pragma warning (disable: 4244)

int p[max], parity, n;
double a[max][max], Det;

/*
void Max( void )
{
  for
} */

void Swap( int *A, int *B )
{
  int tmp = *A;

  *A = *B;
  *B = tmp;
}

void MatrixLoad( char* FileName )
{
 int i, j;
 FILE *f;

 if ((f = fopen("a.txt", "r")) == NULL)
   return;
 fscanf( f, "%d", &n);
 if (n > max)
   n = max;
 for (i = 0; i < n; i++)
   for(j = 0; j < n; j++)
     fscanf(f,"%lf", &a[i][j]);
 fclose(f);
}

void PrintDet( void )
 {
   FILE *f;
 
   f = fopen("OUTPUT.TXT", "a");
   if (f == NULL)
     return;
   fprintf(f, "Det = %lf \n", Det);
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
  int i;

  MatrixLoad("A.txt");
  for (i = 0; i < n; i++)
    p[i] = i;
  Go(0);
  PrintDet();
}







 




