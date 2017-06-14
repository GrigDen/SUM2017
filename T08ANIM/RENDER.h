/* FILE NAME: RENDER.H
 * PROGRAMMER: DG5
 * DATE: 10.06.2017
 * PURPOSE: Base rendering system module.
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "def.h"

/* Project parameters */
extern DBL
                       /* Project plane width */
  DG5_RndProjFarClip,       /* Project plane height */
  DG5_RndProjDist, /* Distance from viewer to project plane */
  DG5_RndProjSize; /* Prohect plane inner size */

extern MATR
  DG5_RndMatrView, /* Viewer matrix */
  DG5_RndMatrProj; /* Projection Matrix */

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
typedef struct tagdg5OBJ3D
{
  VEC *V;      /* Vertex array */
  INT NumOfV;  /* Vertex array size */
  INT (*F)[3]; /* Facets array (point indices) */
  INT NumOfF;  /* Facets array size */
} dg5OBJ3D;

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       dg5OBJ3D *Obj;
 *   - model *.OBJ file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DG5_RndObjLoad( dg5OBJ3D *Obj, CHAR *FileName );

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       dg5OBJ3D *Obj;
 * RETURNS: None.
 */
VOID DG5_RndObjFree( dg5OBJ3D *Obj );

/* Object drawing function.
 * ARGUMENTS:
 *   - object pointer:
 *       dg5OBJ3D *Obj;
 *   - world coordinate system transform matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID DG5_RndObjDraw( dg5OBJ3D *Obj, MATR M );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */