/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_boundingbox_h
#define _dnt_boundingbox_h

#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

/*! The Bounding Box Class */
class boundingBox
{
   public:
      /*! Constructor */
      boundingBox();
      boundingBox(float min[3], float max[3]);
      /*! Destructor */
      ~boundingBox();

      /*! Set min values of the bounding box */
      void setMin(float x, float y, float z);
      void setMin(float m[3]);

      /*! Set max values of the bounding box */
      void setMax(float x, float y, float z);
      void setMax(float m[3]);

      /*! Get min values of the bounding box */
      void getMin(float& x, float& y, float& z);

      /*! Get max values of the bounding box */
      void getMax(float& x, float& y, float& z);
      
      /*! Translate the bounding box to the desired position */
      void translate(float x, float y, float z);

      /*! Apply the rotations to the bounding box */
      void rotate(float angleX, float angleY, float angleZ);

      /*! Verify if the two bounding boxes intercepts or not */
      bool intercepts(boundingBox& b);

      /*! Verify if a bounding box is visible at the viewFrustum matrix */
      bool isVisible(GLfloat** matrix);

      /*! Render the bounding box */
      void render();

      /*! Negate the Y box (to test reflexions, for example) */
      void notY();

      /*! Multiply the bounding box with the 4x4 shadow matrix */
      void multiplyShadow(float** shadowMatrix);

      /*! assign operator */
      void operator=(const boundingBox& v);

      GLfloat x1,
              z1,
              y1,
              x2,
              z2,
              y2;
};

#endif

