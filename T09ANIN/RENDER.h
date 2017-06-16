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

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DG5_RndInit( VOID );

/* Project parameters adjust function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DG5_RndSetProj( VOID );

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

} dg5PRIM;

typedef struct tagdg5OBJ
{
  INT NumOfP;  
  dg5PRIM *P;

} dg5OBJ;
                                                                                                   
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

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */