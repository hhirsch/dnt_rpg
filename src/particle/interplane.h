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

#ifndef _dnt_interplane_h
#define _dnt_interplane_h

#include "../etc/list.h"

#define PLANE_NO_INCLINATION 0 /**< No Inclination on Plane */
#define PLANE_INCLINATION_X  1 /**< Inclination on X axis */
#define PLANE_INCLINATION_Z  2 /**< Inclination on Z axis */

/*! Intersection Plane */
class interPlane: public dntListElement
{
   public:
      float x1,        /**< X coordinate of plane */
            x2;        /**< X coordinate of plane */
      float y1,        /**< Y coordinate of plane */
            y2;        /**< Y coordinate of plane */
      float z1,        /**< Z coordinate of plane */
            z2;        /**< Z coordinate of plane */
      float dX,        /**< Variation X of water on plane */
            dZ;        /**< Variation Z of water on plane */
      int inclination; /**< Inclination Type of plane */
};

/*! The list of intersection planes */
class interPlaneList: public dntList
{
   public:
      /*! Constructor */
      interPlaneList();
      /*! Destructor */
      ~interPlaneList();

      /*! Verify if a particle intersect a plane on the list
       * \param p -> pointer to the particle
       * \param dX -> Variation on X of the plane, if intersected
       * \param dZ -> Variation on Z of the plane, if intersected 
       * \return true if intersected some plane */
      bool intersectPlanes(void* p, float* dX, float* dZ);

   protected:
      /*! Free the interPlane memory
       * \param obj -> pointer to the obj to free */
      void freeElement(dntListElement* obj);
};


#endif


