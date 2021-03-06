/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_part_origin_h
#define _dnt_part_origin_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <string>

#define DNT_PART_ORIGIN_POINT    0  /**< All originated at point */
#define DNT_PART_ORIGIN_QUAD     1  /**< All originated at quad */
#define DNT_PART_ORIGIN_SPHERE   2  /**< All originated at a sphere */

/*! Define the particle system origin, generating initial position
 * for new particles */
class dntPartOrigin
{
   public:
      /*! Constructor */
      dntPartOrigin();

      /*! Set a point to be the particle origin
       * \param x -> X coordinate
       * \param y -> Y coordinate 
       * \param z -> Z coordinate */
      void setPoint(float x, float y, float z);
      /*! Set a quad to be the particle origin
       * \param x1 -> upper X coordinate
       * \param y1 -> upper Y coordinate
       * \param z1 -> upper Z coordinate
       * \param x2 -> lower X coordinate
       * \param y2 -> lower Y coordinate
       * \param z2 -> lower Z coordnate */
      void setQuad(float x1, float y1, float z1, 
                   float x2, float y2, float z2);
      /*! Set an sphere to be the particle origin
       * \param x -> X coordinate
       * \param y -> Y coordinate 
       * \param z -> Z coordinate
       * \param r -> sphere radius */
      void setSphere(float x, float y, float z, float r);

      /*! Generate an initial position for a new particle, according
       * to what is defined as the origin.
       * \param x -> will receive the X coordinate generated
       * \param y -> will receive the Y coordinate generated
       * \param z -> will receive the Z coordinate generated 
       * \return true on success. */
      bool generateNewInitialPosition(float& x, float& y, float& z);

      /*! Update the defined origin to the desired point
       * \param x -> origin new X
       * \param y -> origin new Y
       * \param z -> origin new Z */
      void update(float x, float y, float z);

      /*! Update the defined origin to the desired point
       * \param x -> origin new X
       * \param z -> origin new Z */
      void update(float x, float z);

      /*! Get the current base position of the origin
       * \param x -> will receive X coordinate 
       * \param y -> will receive Y coordinate 
       * \param z -> will receive Z coordinate */
      void getPosition(float& x, float& y, float& z);

      /*! Convert the dntPartOrigin to an string
       * \return string with origin info */
      std::string toString();

      /*! Populate the dntPartOrigin with a descriptive string
       * \param s -> string with origin info 
       * \return true if success */
      bool fromString(std::string s);

      /*! As friend to avoid useless functions */
      friend class partOriginWindow;

   protected:
      float p[3];          /**< Reference Point */
      float sum[3];        /**< Sum values (used at quad) */
      float radius;        /**< radius (used at sphere) */

      int type;            /**< Origin type constant */
};

#endif
