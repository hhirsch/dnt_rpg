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

#ifndef _dnt_maproad_h
#define _dnt_maproad_h

#include "../engine/util.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#ifdef __APPLE__
   #include <SDL_image/SDL_image.h>
#else
   #include <SDL/SDL_image.h>
#endif

#define MAPROAD_NONE                0
#define MAPROAD_DOWN_UP             1
#define MAPROAD_CURVE_DOWN_LEFT     2
#define MAPROAD_CURVE_DOWN_RIGHT    3
#define MAPROAD_CURVE_UP_LEFT       4
#define MAPROAD_CURVE_UP_RIGHT      5
#define MAPROAD_LEFT_RIGHT          6
#define MAPROAD_CURVE_LEFT_DOWN     7
#define MAPROAD_CURVE_LEFT_UP       8
#define MAPROAD_CURVE_RIGHT_DOWN    9
#define MAPROAD_CURVE_RIGHT_UP     10
#define MAPROAD_INTERSECTION       11

/*! Definitions of Roads on Map */
class mapRoad
{
   public:
      /*! Constructor
       * \param numX -> map number of squares on X
       * \param numZ -> map number of squares on Z */
      mapRoad(int numX, int numZ);
      /*! Destructor */
      ~mapRoad();

      /*! Set the position to have a road and actualize adjacents roads.
       * \param posX -> X position
       * \param posZ -> Z position 
       * \param direction -> direction of the road */
      void setRoad(int posX, int posZ, int direction);

      /*! Remove road from position and actualize adjacents roads.
       * \param posX -> X position
       * \param posZ -> Z position */
      void unsetRoad(int posX, int posZ);

      /*! Draw all roads to world */
      void draw();

   private:
      int maxX;           /**< Max X Roads */
      int maxZ;           /**< Max Z Roads */
      int** typeOfRoad;   /**< Type of Road Matrix (Directions) */

      GLuint roadTexture;       /**< Road Texture */
      GLuint roadCurveTexture;  /**< Road Curve Texture */
         
};

#endif

