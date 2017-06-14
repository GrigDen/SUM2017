/* FILE NAME: T04PERM.C
 * PROGRAMMER: DG5
 * DATE: 01.06.2017
 * PURPOSE: Doing permitations.
 */

#include <stdio.h>
#define max 15

int p[max], parity, n;

void Swap( int *A, int *B )
{
  int tmp = *A;

  *A = *B;
  *B = tmp;
}
void PrintPerm( void )
{ 
  int i;
  double z = 1;
  FILE *f;

  f = fopen("Perm.TXT","a");
  if (f == NULL)
    return;
  for (i = 0; i < n; i++)
  {
    fprintf(f, "%i", p[i]);
    z *= p[i]; 
  }
  fprintf(f,"-%s\n", parity ? "odd" : "even");
  fclose(f);
}

void Go( int Pos )
{
  int i;

  if (Pos >= n)
  {
    PrintPerm();
    return;
  }
  for (i = Pos; i < n; i++)
  {  
    Swap(&p[Pos], &p[i]);
    if (Pos != i)
      parity = !parity;
    Go(Pos + 1); 
    Swap(&p[Pos], &p[i]);
    if (Pos != i)
      parity = !parity;
  }
}

void main( void )
{
  int i;

  n = 3;
  parity = 0;
  for (i = 0; i < n; i++)
    p[i] = i + 1;
  Go(0);
}