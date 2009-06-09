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

#ifndef _dnt_culling_h
#define _dnt_culling_h

#include <SDL/SDL_opengl.h>
#include <math.h>

/*! Update the actual view frustum
 * \param matriz -> view frustum matrix (actualized)
 * \param proj -> projection view matrix
 * \param modl -> model view matrix */
void updateFrustum(GLfloat** matriz, GLdouble proj[16],GLdouble modl[16]);

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
                GLfloat** matriz);


#endif

