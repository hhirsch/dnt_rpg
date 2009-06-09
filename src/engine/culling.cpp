/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "culling.h"

/*! Delta value to render, since antialiasing
 * will do "glitch" effects if only render when visible */
#define ANTI_ALIASING_DELTA  20  

/*********************************************************
 *                      updateFrustum                    *
 *********************************************************/
void updateFrustum(GLfloat** matriz,GLdouble proj[16],GLdouble modl[16])
{
   GLfloat clip[16];
	GLfloat t;

	// Get The Current PROJECTION Matrix From OpenGL
	glGetDoublev( GL_PROJECTION_MATRIX, proj );

	// Get The Current MODELVIEW Matrix From OpenGL
	glGetDoublev( GL_MODELVIEW_MATRIX, modl );

	// Combine The Two Matrices (Multiply Projection By Modelview)
	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

	// Extract The Numbers For The RIGHT Plane
	matriz[0][0] = clip[ 3] - clip[ 0];
	matriz[0][1] = clip[ 7] - clip[ 4];
	matriz[0][2] = clip[11] - clip[ 8];
	matriz[0][3] = clip[15] - clip[12];

	// Normalize The Result
	t = GLfloat(sqrt( matriz[0][0] * matriz[0][0] + matriz[0][1] * matriz[0][1] + matriz[0][2] * matriz[0][2] ));
	matriz[0][0] /= t;
	matriz[0][1] /= t;
	matriz[0][2] /= t;
	matriz[0][3] /= t;

	// Extract The Numbers For The LEFT Plane
	matriz[1][0] = clip[ 3] + clip[ 0];
	matriz[1][1] = clip[ 7] + clip[ 4];
	matriz[1][2] = clip[11] + clip[ 8];
	matriz[1][3] = clip[15] + clip[12];

	// Normalize The Result
	t = GLfloat(sqrt( matriz[1][0] * matriz[1][0] + matriz[1][1] * matriz[1][1] + matriz[1][2] * matriz[1][2] ));
	matriz[1][0] /= t;
	matriz[1][1] /= t;
	matriz[1][2] /= t;
	matriz[1][3] /= t;

	// Extract The BOTTOM Plane
	matriz[2][0] = clip[ 3] + clip[ 1];
	matriz[2][1] = clip[ 7] + clip[ 5];
	matriz[2][2] = clip[11] + clip[ 9];
	matriz[2][3] = clip[15] + clip[13];

	// Normalize The Result
	t = GLfloat(sqrt( matriz[2][0] * matriz[2][0] + matriz[2][1] * matriz[2][1] + matriz[2][2] * matriz[2][2] ));
	matriz[2][0] /= t;
	matriz[2][1] /= t;
	matriz[2][2] /= t;
	matriz[2][3] /= t;

	// Extract The TOP Plane
	matriz[3][0] = clip[ 3] - clip[ 1];
	matriz[3][1] = clip[ 7] - clip[ 5];
	matriz[3][2] = clip[11] - clip[ 9];
	matriz[3][3] = clip[15] - clip[13];

	// Normalize The Result
	t = GLfloat(sqrt( matriz[3][0] * matriz[3][0] + matriz[3][1] * matriz[3][1] + matriz[3][2] * matriz[3][2] ));
	matriz[3][0] /= t;
	matriz[3][1] /= t;
	matriz[3][2] /= t;
	matriz[3][3] /= t;

	// Extract The FAR Plane
	matriz[4][0] = clip[ 3] - clip[ 2];
	matriz[4][1] = clip[ 7] - clip[ 6];
	matriz[4][2] = clip[11] - clip[10];
	matriz[4][3] = clip[15] - clip[14];

	// Normalize The Result
	t = GLfloat(sqrt( matriz[4][0] * matriz[4][0] + matriz[4][1] * matriz[4][1] + matriz[4][2] * matriz[4][2] ));
	matriz[4][0] /= t;
	matriz[4][1] /= t;
	matriz[4][2] /= t;
	matriz[4][3] /= t;

	// Extract The NEAR Plane
	matriz[5][0] = clip[ 3] + clip[ 2];
	matriz[5][1] = clip[ 7] + clip[ 6];
	matriz[5][2] = clip[11] + clip[10];
	matriz[5][3] = clip[15] + clip[14];

	// Normalize The Result
	t = GLfloat(sqrt( matriz[5][0] * matriz[5][0] + matriz[5][1] * matriz[5][1] + matriz[5][2] * matriz[5][2] ));
	matriz[5][0] /= t;
	matriz[5][1] /= t;
	matriz[5][2] /= t;
	matriz[5][3] /= t;

};

/*********************************************************
 *                      visibleCube                      *
 *********************************************************/
int visibleCube(GLfloat x1, GLfloat y1, GLfloat z1,
                GLfloat x2, GLfloat y2, GLfloat z2,
                GLfloat** matriz)
{
   /* Apply AntiAliasing Delta */
   x1 = x1 - ANTI_ALIASING_DELTA;
   y1 = y1 - ANTI_ALIASING_DELTA;
   z1 = z1 - ANTI_ALIASING_DELTA;
   x2 = x2 + ANTI_ALIASING_DELTA;
   y2 = y2 + ANTI_ALIASING_DELTA;
   z2 = z2 + ANTI_ALIASING_DELTA;

   /* If all points are outter planes, is out! */
   if(((matriz[0][0]*x1+matriz[0][1]*y1+matriz[0][2]*z1+matriz[0][3] <= 0) &&
       (matriz[0][0]*x1+matriz[0][1]*y1+matriz[0][2]*z2+matriz[0][3] <= 0) &&
       (matriz[0][0]*x1+matriz[0][1]*y2+matriz[0][2]*z1+matriz[0][3] <= 0) &&
       (matriz[0][0]*x1+matriz[0][1]*y2+matriz[0][2]*z2+matriz[0][3] <= 0) &&
       (matriz[0][0]*x2+matriz[0][1]*y1+matriz[0][2]*z1+matriz[0][3] <= 0) &&
       (matriz[0][0]*x2+matriz[0][1]*y1+matriz[0][2]*z2+matriz[0][3] <= 0) &&
       (matriz[0][0]*x2+matriz[0][1]*y2+matriz[0][2]*z1+matriz[0][3] <= 0) &&
       (matriz[0][0]*x2+matriz[0][1]*y2+matriz[0][2]*z2+matriz[0][3] <= 0)) ||
      ((matriz[1][0]*x1+matriz[1][1]*y1+matriz[1][2]*z1+matriz[1][3] <= 0) &&
       (matriz[1][0]*x1+matriz[1][1]*y1+matriz[1][2]*z2+matriz[1][3] <= 0) &&
       (matriz[1][0]*x1+matriz[1][1]*y2+matriz[1][2]*z1+matriz[1][3] <= 0) &&
       (matriz[1][0]*x1+matriz[1][1]*y2+matriz[1][2]*z2+matriz[1][3] <= 0) &&
       (matriz[1][0]*x2+matriz[1][1]*y1+matriz[1][2]*z1+matriz[1][3] <= 0) &&
       (matriz[1][0]*x2+matriz[1][1]*y1+matriz[1][2]*z2+matriz[1][3] <= 0) &&
       (matriz[1][0]*x2+matriz[1][1]*y2+matriz[1][2]*z1+matriz[1][3] <= 0) &&
       (matriz[1][0]*x2+matriz[1][1]*y2+matriz[1][2]*z2+matriz[1][3] <= 0)) ||
      ((matriz[2][0]*x1+matriz[2][1]*y1+matriz[2][2]*z1+matriz[2][3] <= 0) &&
       (matriz[2][0]*x1+matriz[2][1]*y1+matriz[2][2]*z2+matriz[2][3] <= 0) &&
       (matriz[2][0]*x1+matriz[2][1]*y2+matriz[2][2]*z1+matriz[2][3] <= 0) &&
       (matriz[2][0]*x1+matriz[2][1]*y2+matriz[2][2]*z2+matriz[2][3] <= 0) &&
       (matriz[2][0]*x2+matriz[2][1]*y1+matriz[2][2]*z1+matriz[2][3] <= 0) &&
       (matriz[2][0]*x2+matriz[2][1]*y1+matriz[2][2]*z2+matriz[2][3] <= 0) &&
       (matriz[2][0]*x2+matriz[2][1]*y2+matriz[2][2]*z1+matriz[2][3] <= 0) &&
       (matriz[2][0]*x2+matriz[2][1]*y2+matriz[2][2]*z2+matriz[2][3] <= 0)) ||
      ((matriz[3][0]*x1+matriz[3][1]*y1+matriz[3][2]*z1+matriz[3][3] <= 0) &&
       (matriz[3][0]*x1+matriz[3][1]*y1+matriz[3][2]*z2+matriz[3][3] <= 0) &&
       (matriz[3][0]*x1+matriz[3][1]*y2+matriz[3][2]*z1+matriz[3][3] <= 0) &&
       (matriz[3][0]*x1+matriz[3][1]*y2+matriz[3][2]*z2+matriz[3][3] <= 0) &&
       (matriz[3][0]*x2+matriz[3][1]*y1+matriz[3][2]*z1+matriz[3][3] <= 0) &&
       (matriz[3][0]*x2+matriz[3][1]*y1+matriz[3][2]*z2+matriz[3][3] <= 0) &&
       (matriz[3][0]*x2+matriz[3][1]*y2+matriz[3][2]*z1+matriz[3][3] <= 0) &&
       (matriz[3][0]*x2+matriz[3][1]*y2+matriz[3][2]*z2+matriz[3][3] <= 0)) ||
      ((matriz[4][0]*x1+matriz[4][1]*y1+matriz[4][2]*z1+matriz[4][3] <= 0) &&
       (matriz[4][0]*x1+matriz[4][1]*y1+matriz[4][2]*z2+matriz[4][3] <= 0) &&
       (matriz[4][0]*x1+matriz[4][1]*y2+matriz[4][2]*z1+matriz[4][3] <= 0) &&
       (matriz[4][0]*x1+matriz[4][1]*y2+matriz[4][2]*z2+matriz[4][3] <= 0) &&
       (matriz[4][0]*x2+matriz[4][1]*y1+matriz[4][2]*z1+matriz[4][3] <= 0) &&
       (matriz[4][0]*x2+matriz[4][1]*y1+matriz[4][2]*z2+matriz[4][3] <= 0) &&
       (matriz[4][0]*x2+matriz[4][1]*y2+matriz[4][2]*z1+matriz[4][3] <= 0) &&
       (matriz[4][0]*x2+matriz[4][1]*y2+matriz[4][2]*z2+matriz[4][3] <= 0)) ||
      ((matriz[5][0]*x1+matriz[5][1]*y1+matriz[5][2]*z1+matriz[5][3] <= 0) &&
       (matriz[5][0]*x1+matriz[5][1]*y1+matriz[5][2]*z2+matriz[5][3] <= 0) &&
       (matriz[5][0]*x1+matriz[5][1]*y2+matriz[5][2]*z1+matriz[5][3] <= 0) &&
       (matriz[5][0]*x1+matriz[5][1]*y2+matriz[5][2]*z2+matriz[5][3] <= 0) &&
       (matriz[5][0]*x2+matriz[5][1]*y1+matriz[5][2]*z1+matriz[5][3] <= 0) &&
       (matriz[5][0]*x2+matriz[5][1]*y1+matriz[5][2]*z2+matriz[5][3] <= 0) &&
       (matriz[5][0]*x2+matriz[5][1]*y2+matriz[5][2]*z1+matriz[5][3] <= 0) &&
       (matriz[5][0]*x2+matriz[5][1]*y2+matriz[5][2]*z2+matriz[5][3] <= 0)))

   {
      return(0);
   }
   else
   {
      /* if a point is inner, so its visible */
      return(1);
   }

}

