#ifndef _culling_h
#define _culling_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include <SDL/SDL_opengl.h>
#include <math.h>

/*! Actualize the actual view frustum
 * \param matriz -> view frustum matrix (actualized)
 * \param proj -> projection view matrix
 * \param modl -> model view matrix */
void actualizeFrustum(GLfloat matriz[6][4],GLdouble proj[16],GLdouble modl[16]);

/*!
 * Verify if the square is in frustum or not.
 * \param x1 -> upper X coordinate
 * \param y1 -> upper Y coordinate
 * \param z1 -> upper Z coordinate
 * \param x2 -> lower X coordinate
 * \param y2 -> lower Y coordinate
 * \param z2 -> lower Z coordinate  
 * \param matriz -> view Frustum Matrix */
int visibleCube(GLfloat x1,GLfloat y1,GLfloat z1,
                GLfloat x2,GLfloat y2,GLfloat z2,
                GLfloat matriz[6][4]);


#endif

