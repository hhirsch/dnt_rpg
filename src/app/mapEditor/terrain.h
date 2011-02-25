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

#ifndef _dnt_map_editor_terrain_h
#define _dnt_map_editor_terrain_h

#include "../../map/map.h"
#include "message.h"


class terrain
{
   public:
      /*! Constructor
       * \param acMap -> pointer to opened map */
      terrain(Map* acMap);
      /*! Destructor */
      ~terrain();

      /*! Verify and do action by mouse and state 
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate 
       * \param mouseZ -> mouse Z coordinate
       * \param floorX -> X mouse on floor
       * \param floorZ -> Z mouse on floor
       * \param mButton -> mouse button state
       * \param tool -> current Tool
       * \param actualTexture -> current Texture */
      void verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        GLfloat floorX, GLfloat floorZ,
                        Uint8 mButton, int tool, GLuint actualTexture);

      /*! If have some temporary things to draw, draw it! */
      void drawTemporary();

   private:
      Map* actualMap;   /**< Actual Internal Map */
      int actualTool;   /**< Actual Tool */
      int state;        /**< Actual Internal State */

      int quadX;        /**< Mouse Mouse Square X Coordinate */
      int quadZ;        /**< Mouse Mouse Square Z Coordinate */

      int quadInitX;    /**< X quad value at init of some state */
      int quadInitZ;    /**< Y quad value at init of some state */

      GLfloat height;      /**< Current Map Height On Mouse */
      GLfloat nivelHeight; /**< Map Height at init of Nivelate */

      GLfloat mX, mY, mZ;  /**< Mouse Positions */
 
      GLfloat initmX, initmY, initmZ; /**< Mouse Positions On Init of State */

      /*! Do up Down on Terrain */
      void doUpDown(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                    Uint8 mButton);
      /*! Do Nivelate on Terrain */
      void doNivelate(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                      Uint8 mButton);
      /*! Do Change Texture on Terrain */
      void doTexture(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ,
                     Uint8 mButton, GLuint actualTexture );
      /*! Verifies if point is in Square */
      bool pointInSquare(GLfloat x, GLfloat y, 
                            GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
};


#endif

