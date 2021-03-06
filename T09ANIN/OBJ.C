/* FILE NAME: OBJ.C
 * PROGRAMMER: DG5
 * DATE: 14.06.2017
 * PURPOSE: Object handle module.
 */

#include <stdio.h>

#include "anim.h"

/* Create object function.
 * ARGUMENTS:
 *   - created object:
 *       dg5OBJ *Obj;
 *   - number of primitives:
 *       INT NumOfP;
* RETURNS: None.
 */
VOID DG5_RndObjCreate( dg5OBJ *Obj, INT NumOfP )
{
  memset(Obj, 0, sizeof(dg5PRIM));
  if ((Obj->P = malloc(sizeof(dg5PRIM) * NumOfP)) == NULL)
    return;
  memset(Obj->P, 0, sizeof(dg5PRIM) * NumOfP);
  Obj->NumOfP = NumOfP;
} /* Ens of 'DG5_RndObjCreate' function */

/* Free object function.
 * ARGUMENTS:
 *   - deleted object:
 *       dg5OBJ *Obj;
 * RETURNS: None.
 */
VOID DG5_RndObjFree( dg5OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfP; i++)
    DG5_RndPrimFree(&Obj->P[i]);
  free(Obj->P);
  memset(Obj, 0, sizeof(dg5PRIM));
} /* Ens of 'DG5_RndObjFree' function */

/* Draw object function.
 * ARGUMENTS:
 *   - drawing object:
 *       dg5OBJ *Obj;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID DG5_RndObjDraw( dg5OBJ *Obj, MATR M )
{
  INT i;

  for (i = 0; i < Obj->NumOfP; i++)
    DG5_RndPrimDraw(&Obj->P[i], M);
} /* Ens of 'DG5_RndObjDraw' function */

/* Load object from "G3DM" file function.
 * ARGUMENTS:
 *   - drawing object:
 *       dg5OBJ *Obj;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DG5_RndObjLoad( dg5OBJ *Obj, CHAR *FileName )
{
  FILE *F;
  DWORD Sign;
  INT NoofP = 0, NoofM = 0, NoofT = 0, i;

  memset(Obj, 0, sizeof(dg5OBJ));

  /* Open file */
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  /* Check signature */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3DM")
  {
    fclose(F);
    return FALSE;
  }

  /* Read quantities */
  fread(&NoofP, 4, 1, F);
  fread(&NoofM, 4, 1, F);
  fread(&NoofT, 4, 1, F);

  if (DG5_RndNumOfMaterials + NoofM >= DG5_RES_TABLE_SIZE ||
      DG5_RndNumOfTextures + NoofT >= DG5_RES_TABLE_SIZE)
  {
    fclose(F);
    return FALSE;
  }

  /* Create object */
  DG5_RndObjCreate(Obj, NoofP);

  /* Load primitives */
  for (i = 0; i < NoofP; i++)
  {
    INT nv = 0, ni = 0, mtl_no = -1, size;
    dg5VERTEX *V;
    INT *I;

    fread(&nv, 4, 1, F);
    fread(&ni, 4, 1, F);
    fread(&mtl_no, 4, 1, F);
    size = sizeof(dg5VERTEX) * nv + sizeof(INT) * ni;
    if ((V = malloc(size)) == NULL)
    {
      fclose(F);
      DG5_RndObjFree(Obj);
      return FALSE;
    }
    fread(V, 1, size, F);
    I = (INT *)(V + nv);
    DG5_RndPrimCreate(&Obj->P[i], TRUE, V, nv, I, ni);
    free(V);
    if (mtl_no != -1)
      mtl_no += DG5_RndNumOfMaterials;
    Obj->P[i].MtlNo = mtl_no;
  }

  /* Load materials */
  fread(&DG5_RndMaterials[DG5_RndNumOfMaterials], sizeof(dg5MATERIAL), NoofM, F);
  for (i = 0; i < NoofM; i++)
  {
    INT j;
    dg5MATERIAL *mtl = &DG5_RndMaterials[DG5_RndNumOfMaterials + i];

    if (mtl->ShaderStr[0] != 0)
      mtl->Shader = DG5_RndShaderAdd(mtl->ShaderStr);

    for (j = 0; j < 8; j++)
      if (mtl->Tex[j] != -1)
        mtl->Tex[j] += DG5_RndNumOfTextures;
  }
  DG5_RndNumOfMaterials += NoofM;

  /* Load textures */
  for (i = 0; i < NoofT; i++)
  {
    CHAR Name[300];
    INT W, H, C;
    VOID *mem;

    fread(Name, 1, 300, F);
    fread(&W, 4, 1, F);
    fread(&H, 4, 1, F);
    fread(&C, 4, 1, F);
    if ((mem = malloc(W * H * C)) == NULL)
    {
      fclose(F);
      DG5_RndObjFree(Obj);
      return FALSE;
    }
    fread(mem, C, W * H, F);
    DG5_RndTextureAdd(Name, W, H, C, mem);
    free(mem);
  }
  DG5_RndNumOfTextures += NoofT;

  fclose(F);
  return TRUE;
} /* Ens of 'DG5_RndObjLoad' function */

/* END OF 'OBJ.C' FILE */
