/* FILE NAME: T04PERM.C
 * PROGRAMMER: DG5
 * DATE: 01.06.2017
 * PURPOSE: Doing permitations.
 */

#include <stdio.h>
#define max  50
int p[max];

void PrintPerm( void )
{ 
  int i;
  FILE *F;
  f = fopen("Perm.TXT","a");
  if (f == NULL)
    return;
  for (i = 0; i < n; i++)
    fprintf(f, "%i", p[i]);
  fprintf(f,"-%s\n", parity ?"odd" : "even"
  fclose(f);
}

void Go( int Pos )
{
  int i;
  if (Pos >= n)
    return;
  for (i = Pos; i < n; i++)
  {  
    Swap(&p[pos], &p[i]);
    Go(Pos + 1);
    Swap((&p[Pos], &p[i]);
  }
}

void Swap( *A, *B)
{
  int tmp = *A;
  *A = *B;
  *B = tmp;
}

void main( void )
{
  int n, i;
  scanf("%i", &n);
  for (i = 0; i < n; i++)
    p[i] = i + 1;
  Go(0);
}