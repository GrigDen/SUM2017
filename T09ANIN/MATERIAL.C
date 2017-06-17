/* FILE NAME: MATERIAL.C
 * PROGRAMMER: DG5
 * DATE: 17.06.2017
 * PURPOSE: Material (texture, shader store) handle module.
 */

#include <string.h>

#include "anim.h"

/***
 * Resource storages
 ***/

/* Materials table */
dg5MATERIAL DG5_RndMaterials[DG5_RES_TABLE_SIZE];
INT DG5_RndNumOfMaterials;

/* Textures table */
dg5TEXTURE DG5_RndTextures[DG5_RES_TABLE_SIZE];
INT DG5_RndNumOfTextures;

/* Shaders table */
dg5SHADER DG5_RndShaders[DG5_RES_TABLE_SIZE];
INT DG5_RndNumOfShaders;

/* Resourcse initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DG5_RndResInit( VOID )
{
  dg5MATERIAL def_mtl =
  {
    "Default material",
    {0.1, 0.1, 0.1}, {0.9, 0.9, 0.9}, {0.3, 0.3, 0.3}, 30,
    1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    "", 0
  };

  memset(DG5_RndMaterials, 0, sizeof(DG5_RndMaterials));
  memset(DG5_RndTextures, 0, sizeof(DG5_RndTextures));
  memset(DG5_RndShaders, 0, sizeof(DG5_RndShaders));
  DG5_RndNumOfMaterials = 0;
  DG5_RndNumOfTextures = 0;
  DG5_RndNumOfShaders = 0;

  DG5_RndShaderAdd("SHADERS\\DEFAULT");
  DG5_RndMaterialAdd(&def_mtl);
} /* End of 'DG5_RndResInit' function */

/* Resourcse deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DG5_RndResClose( VOID )
{
  INT i;

  /* Delete shaders */
  for (i = 0; i < DG5_RndNumOfShaders; i++)
    DG5_RndShaderFree(DG5_RndShaders[i].ProgId);
  memset(DG5_RndShaders, 0, sizeof(DG5_RndShaders));
  DG5_RndNumOfShaders = 0;

  /* Delete textures */
  for (i = 0; i < DG5_RndNumOfTextures; i++)
    glDeleteTextures(1, &DG5_RndTextures[i].TexId);
  memset(DG5_RndTextures, 0, sizeof(DG5_RndTextures));
  DG5_RndNumOfTextures = 0;

  /* Remove materials */
  memset(DG5_RndMaterials, 0, sizeof(DG5_RndMaterials));
  DG5_RndNumOfMaterials = 0;
} /* End of 'DG5_RndResClose' function */

/* Add shader function.
 * ARGUMENTS:
 *   - shader file name prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (INT) shader table number.
 */
INT DG5_RndShaderAdd( CHAR *FileNamePrefix )
{
  if (DG5_RndNumOfShaders >= DG5_RES_TABLE_SIZE)
    return 0;
  strncpy(DG5_RndShaders[DG5_RndNumOfShaders].Name, FileNamePrefix,
    DG5_MATERIAL_NAME_SIZE - 1);
  DG5_RndShaders[DG5_RndNumOfShaders].ProgId =
    DG5_RndShaderLoad(FileNamePrefix);
  return DG5_RndNumOfShaders++;
} /* End of 'DG5_RndShaderAdd' function */

/* Add material function.
 * ARGUMENTS:
 *   - material data:
 *       dg5MATERIAL *Mtl;
 * RETURNS:
 *   (INT) material table number.
 */
INT DG5_RndMaterialAdd( dg5MATERIAL *Mtl )
{
  if (DG5_RndNumOfMaterials >= DG5_RES_TABLE_SIZE)
    return 0;
  DG5_RndMaterials[DG5_RndNumOfMaterials] = *Mtl;
  return DG5_RndNumOfMaterials++;
} /* End of 'DG5_RndMaterialAdd' function */

/* Add texture function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *Name;
 *   - texture image size:
 *       INT W, H;
 *   - texture image pixel number of component:
 *       INT Components;
 *   - texture image pixel color data:
 *       VOID *Data;
 * RETURNS:
 *   (INT) texture table number.
 */
INT DG5_RndTextureAdd( CHAR *Name, INT W, INT H, INT Components, VOID *Data )
{
  if (DG5_RndNumOfTextures >= DG5_RES_TABLE_SIZE)
    return -1;
  strncpy(DG5_RndTextures[DG5_RndNumOfTextures].Name, Name,
    DG5_MATERIAL_NAME_SIZE - 1);
  DG5_RndTextures[DG5_RndNumOfTextures].W = W;
  DG5_RndTextures[DG5_RndNumOfTextures].H = H;

  glGenTextures(1, &DG5_RndTextures[DG5_RndNumOfTextures].TexId);
  glBindTexture(GL_TEXTURE_2D, DG5_RndTextures[DG5_RndNumOfTextures].TexId);
  gluBuild2DMipmaps(GL_TEXTURE_2D, Components, W, H,
    Components == 4 ? GL_BGRA_EXT : Components == 3 ? GL_BGR_EXT : GL_LUMINANCE,
    GL_UNSIGNED_BYTE, Data);
  glBindTexture(GL_TEXTURE_2D, 0);

  return DG5_RndNumOfTextures++;
} /* End of 'DG5_RndTextureAdd' function */

/* Material apply function.
 * ARGUMENTS:
 *   - material table number:
 *       INT MtlNo;
 * RETURNS:
 *   (UINT) shader program Id.
 */
UINT DG5_RndMaterialApply( INT MtlNo )
{
  dg5MATERIAL *mtl;
  INT prg, loc, i;

  if (MtlNo < 0 || MtlNo >= DG5_RndNumOfMaterials)
    MtlNo = 0;
  mtl = &DG5_RndMaterials[MtlNo];

  /* Enable shader */
  prg = mtl->Shader;
  if (prg < 0 || prg >= DG5_RndNumOfShaders)
    prg = 0;
  prg = DG5_RndShaders[prg].ProgId;

  glUseProgram(prg);

  /* Illumination coefficients */
  loc = glGetUniformLocation(prg, "Ka");
  if (loc != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  loc = glGetUniformLocation(prg, "Kd");
  if (loc != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  loc = glGetUniformLocation(prg, "Ks");
  if (loc != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  loc = glGetUniformLocation(prg, "Ph");
  if (loc != -1)
    glUniform1f(loc, mtl->Ph);
  loc = glGetUniformLocation(prg, "Trans");
  if (loc != -1)
    glUniform1f(loc, mtl->Trans);

  /* Textures */
  for (i = 0; i < 8; i++)
  {
    CHAR ist[] = "IsTexture0";
    INT t = mtl->Tex[i];

    /* Set texture usage flag */
    ist[9] = '0' + i;
    loc = glGetUniformLocation(prg, ist);
    if (loc != -1)
      glUniform1i(loc, t != -1);

    /* Select texture to sampler */
    if (t != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, DG5_RndTextures[t].TexId);
    }
  }
  return prg;
} /* End of 'DG5_RndMaterialApply' function */

/* END OF 'MATERIAL.C' FILE */
