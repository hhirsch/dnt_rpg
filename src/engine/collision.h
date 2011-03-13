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

#ifndef _dnt_collision_h
#define _dnt_collision_h

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <math.h>
#include "../map/map.h"
#include "character.h"

/*! The collision Detection Class */
class collision
{
   public:
      /*! Constructor */
      collision();
      /*! Destructor */
      ~collision();

      /*! Define Current Map
       * \param usedMap -> pointer to current Map
       * \param npcs -> list of NPCs
       * \param pcs -> list of PCs */
      void defineMap(Map* usedMap, characterList* npcs, characterList* pcs);

      /*! Verify if character can occupy position on Square
       * \param min -> min positions of Bounding Box (translated & rotated)
       * \param max -> max positions of bounding box (translated & rotated)
       * \param quad -> pointer to the current square
       * \param varHeight -> height variation going in */
      bool verifySquare(boundingBox& actorBox, Square* quad,
                        GLfloat& varHeight, GLfloat curHeight);

      /*! Verify if character can walk to the position
       *  \param perX -> character X position
       *  \param perY -> character Y position
       *  \param perZ -> character Z position
       *  \param perX1 -> character bounding box X1
       *  \param perY1 -> character bounding box Y1
       *  \param perZ1 -> character bounding box Z1
       *  \param perX2 -> character bounding box X2
       *  \param perY2 -> character bounding box Y2
       *  \param perZ2 -> character bounding box Z2 
       *  \param perOrientation -> character Orientation
       *  \param perQuad -> square occupied by character 
       *  \param varHeight -> value of the new height of the character 
       *  \param nx -> value of the center new X Character
       *  \param nz -> value of the center new Z Character
       *  \return true if the character can occupy the position, 
       *          false otherwise.*/
      /*bool canWalk(GLfloat perX, GLfloat perY, GLfloat perZ, 
                   GLfloat perX1, GLfloat perY1, GLfloat perZ1, 
                   GLfloat perX2, GLfloat perY2, GLfloat perZ2, 
                   GLfloat perOrientation, Square* perQuad,
                   GLfloat& varHeight, GLfloat& nx, GLfloat& nz);*/
      bool canWalk(character* actor, GLfloat varX, GLfloat varY ,GLfloat varZ,
                   GLfloat varAlpha, GLfloat& varHeight, GLfloat& nx, 
                   GLfloat& nz, bool usePosition=true);
                   
      
   private:
      Map* actualMap;         /**< Pointer to current opened map */
      characterList* NPCs;    /**< Pointer to the current NPC List */
      characterList* PCs;     /**< Pointer to the current PC List */
};


#endif

