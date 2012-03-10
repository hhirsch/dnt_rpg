/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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
#include "../etc/quaternion.h"

#define DNT_INFINITE_RAY_SIZE -1

/*! A simple ray */
class ray
{
   public:
      ray()
      {
         size = DNT_INFINITE_RAY_SIZE;
      };

      vec3_t origin;
      vec3_t direction;
      float size;
};

/*! The Bounding Box Class */
class boundingBox
{
   public:
      /*! Constructor */
      boundingBox();
      boundingBox(float minV[3], float maxV[3]);
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

      /*! Verify if bounding box intercepts with a ray */
      bool intercepts(const ray& r, float* d=NULL);

      /*! Verify if a bounding box is visible at the viewFrustum matrix */
      bool isVisible(GLfloat** matrix);

      /*! Render the bounding box */
      void render();

      /*! Negate the Y box (to test reflexions, for example) */
      void notY();

      /*! Multiply the bounding box with the 4x4 shadow matrix */
      void multiplyShadow(float* shadowMatrix);

      /*! assign operator */
      void operator=(const boundingBox& v);

      vec3_t min;
      vec3_t max;
};

#endif

