/* FILE NAME: RENDER.H
 * PROGRAMMER: DG5
 * DATE: 10.06.2017
 * PURPOSE: Base rendering system module.
 */

#ifndef __RENDER_H_
#define __RENDER_H_

/* OpenGL specific includes */
#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "def.h"

/***
 * Material support
 ***/
#define DG5_RES_TABLE_SIZE 1000

#define DG5_MATERIAL_NAME_SIZE 300

/* Shader store representation type */
typedef struct tagdg5SHADER
{
  CHAR Name[DG5_MATERIAL_NAME_SIZE]; /* Shader file name prefix */
  UINT ProgId;                       /* Shader program ID */
} dg5SHADER;

/* Texture store representation type */
typedef struct tagdg5TEXTURE
{
  CHAR Name[DG5_MATERIAL_NAME_SIZE]; /* Texture file name */
  INT W, H;                          /* Texture image size */
  UINT TexId;                        /* Texture ID */
} dg5TEXTURE;

/* Material store representation type */
typedef struct tagdg5MATERIAL
{
  CHAR Name[DG5_MATERIAL_NAME_SIZE]; /* Material name */

  /* Illumination coefficients */ 
  VEC Ka, Kd, Ks;                    /* Ambient, diffuse, specular coefficients */
  FLT Ph;                            /* Phong power coefficient */
  FLT Trans;                         /* Transparency factor */
  INT Tex[8];                        /* Texture references */

  /* Shader string */
  CHAR ShaderStr[DG5_MATERIAL_NAME_SIZE];
  INT Shader;                        /* Texture references */
} dg5MATERIAL;

/* Materials table */
extern dg5MATERIAL DG5_RndMaterials[DG5_RES_TABLE_SIZE];
extern INT DG5_RndNumOfMaterials;

/* Textures table */
extern dg5TEXTURE DG5_RndTextures[DG5_RES_TABLE_SIZE];
extern INT DG5_RndNumOfTextures;

/* Shaders table */
extern dg5SHADER DG5_RndShaders[DG5_RES_TABLE_SIZE];
extern INT DG5_RndNumOfShaders;


/* Project parameters */
extern FLT
                       /* Project plane width */
  DG5_RndProjFarClip,       /* Project plane height */
  DG5_RndProjDist, /* Distance from viewer to project plane */
  DG5_RndProjSize; /* Prohect plane inner size */
extern MATR
  DG5_RndMatrView, /* Viewer matrix */
  DG5_RndMatrProj; /* Projection Matrix */
extern UINT 
  DG5_RndProgId; /* Shader program indentifier */
extern VEC 
  DG5_RndLightColor,
  DG5_RndLightPos;


/***
 * Object handle
 ***/

/* Object description type */
typedef struct tagdg5VERTEX
{
  VEC P;
  VEC2 T;
  VEC N;
  VEC4 C;
     
}dg5VERTEX;

typedef struct tagdg5PRIM
{
  BOOL IsTrimesh;
  INT NumOfV;
  INT NumOfI;
  MATR M;
  INT VA, VBuf;
  INT IBuf;
  INT MtlNo;
} dg5PRIM;

typedef struct tagdg5OBJ
{
  INT NumOfP;  
  dg5PRIM *P;

} dg5OBJ;

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DG5_RndInit( VOID );

/* Project parameters adjust function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DG5_RndSetProj( VOID ); /* Resourcse initialization function.
 
/* Resourcse deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DG5_RndResClose( VOID );

/* Add shader function.
 * ARGUMENTS:
 *   - shader file name prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (INT) shader table number.
 */
INT DG5_RndShaderAdd( CHAR *FileNamePrefix );

/* Add material function.
 * ARGUMENTS:
 *   - material data:
 *       dg5MATERIAL *Mtl;
 * RETURNS:
 *   (INT) material table number.
 */
INT DG5_RndMaterialAdd( dg5MATERIAL *Mtl );



/* Material apply function.
 * ARGUMENTS:
 *   - material table number:
 *       INT MtlNo;
 * RETURNS:
 *   (UINT) shader program Id.
 */
UINT DG5_RndMaterialApply( INT MtlNo );

   
/* Create primitive function.
 * ARGUMENTS:
 *   - created primitive:
 *       dg5PRIM *Pr;
 *   - trimesh flag:
 *       BOOL IsTrimesh;
 *   - vertex array:
 *       dg5VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV
 *   - index array:
 *       INT *I;
 *   - index array size:
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID DG5_RndPrimCreate( dg5PRIM *Pr, BOOL IsTrimesh,
                        dg5VERTEX *V, INT NumOfV,
                        INT *I, INT NumOfI );
/* Load shader program function.
 * ARGUMENTS:
 *   - shader files prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (UINT) shader program index or 0 if error is occured.
 */
UINT DG5_RndShaderLoad( CHAR *FileNamePrefix );

/* Shader program load function.
 * ARGUMENTS:
 *   - shader program Id:
 *       UINT Prg;
 * RETURNS: None.
 */
VOID DG5_RndShaderFree( UINT Prg );

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       dg5OBJ3D *Obj;
 *   - model *.OBJ file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DG5_RndPrimLoad( dg5PRIM *Obj, CHAR *FileName );

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       dg5OBJ3D *Obj;
 * RETURNS: None.
 */
VOID DG5_RndPrimFree( dg5PRIM *Obj );

/* Object drawing function.
 * ARGUMENTS:
 *   - object pointer:
 *       dg5OBJ3D *Obj;
 *   - world coordinate system transform matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID DG5_RndPrimDraw( dg5PRIM *Pr, MATR M );

VOID DG5_RndObjCreate( dg5OBJ *Obj, INT NumOfP );

VOID DG5_RndObjFree( dg5OBJ *Obj );


VOID DG5_RndObjDraw( dg5OBJ *Obj, MATR M );
/* Plane primitive creation function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       dg5PRIM *Pr;
 *   - plane corner position:
 *       VEC C;
 *   - plane directions:
 *       VEC Du, Dv;
 *   - plane split numbers:
 *       INT N, M;
 * RETURNS: None.
 */
VOID DG5_RndPrimCreatePlane( dg5PRIM *Pr, VEC C, VEC Du, VEC Dv, INT N, INT M );
/* Sphere primitive creation function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       dg5PRIM *Pr;
 *   - sphere center position:
 *       VEC C;
 *   - sphere radius:
 *       FLT R;
 *   - sphere split numbers:
 *       INT N, M;
 * RETURNS: None.
 */

VOID DG5_RndPrimCreateSphere( dg5PRIM *Pr, VEC C, FLT R, INT N, INT M );
/* Load shader program function.
 * ARGUMENTS:
 *   - shader files prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (UINT) shader program index or 0 if error is occured.
 */
UINT DG5_RndShaderLoad( CHAR *FileNamePrefix );
/* Shader program load function.
 * ARGUMENTS:
 *   - shader program Id:
 *       UINT Prg;
 * RETURNS: None.
 */
VOID DG5_RndShaderFree( UINT Prg );
/* Evaluate grid vertex normals function.
 * ARGUMENTS:
 *   - vertex array:
 *       dg5VERTEX *V;
 *   - grid split numbers:
 *       INT N, M;
 * RETURNS: None.
 */
VOID DG5_RndGridEvalNormals( dg5VERTEX *V, INT N, INT M );
/* Evaluate trimesh vertex normals function.
 * ARGUMENTS:
 *   - vertex array:
 *       dg5VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV;
 *   - index array:
 *       INT *I;
 *   - index array size:
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID DG5_RndTriMeshEvalNormals( dg5VERTEX *V, INT NumOfV, INT *I, INT NumOfI );
/* Load object from "G3DM" file function.
 * ARGUMENTS:
 *   - drawing object:
 *       dg5OBJ *Obj;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DG5_RndObjLoad( dg5OBJ *Obj, CHAR *FileName );

/* Resourcse initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DG5_RndResInit( VOID );

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
INT DG5_RndTextureAdd( CHAR *Name, INT W, INT H, INT Components, VOID *Data );



#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */