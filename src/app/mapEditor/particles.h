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

#ifndef _dnt_map_editor_particles_h
#define _dnt_map_editor_particles_h

#include "grasswindow.h"
#include "waterwindow.h"
#include "message.h"
#include "guiIO.h"
#include "../../map/map.h"
#include "../../particle/partcontroller.h"

namespace dntMapEditor
{

class Particles
{
   public:
      /*! Constructor
       * \param acMap -> poiter to opened map*/
      Particles(Map* acMap);
      /*! Destructor */
      ~Particles();

      /*! Verify and do action by mouse and state 
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate 
       * \param mouseZ -> mouse Z coordinate 
       * \param mButton -> mouse button state
       * \param grWindow -> the grassWindow Used*/
      void verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, Uint8* keys, GuiIO* gui, 
                        partController* pS,
                        GLdouble proj[16],GLdouble modl[16],GLint viewPort[4],
                        std::string selectedText, GrassWindow* grWindow,
                        WaterWindow* wtWindow, Map* actualMap);

      /*! If have some temporary things to draw, draw it! */
      void drawTemporary(GLfloat** matriz);

      void deleteParticle();

   private:
      Map* actualMap;                  /**< actual Opened Map */
      particleSystem* actualParticle;  /**< actual Particle */
      int state;                       /**< actual State */
      int particleType;                /**< actual Particle Type */
      GLfloat height;                  /**< actual Particle Height */

      std::string previousText;        /**< Previous selectedText */

      GLfloat x1,                      /**< X1 coordinate for grass */
              z1,                      /**< Z1 Coordinate for grass */
              x2,                      /**< X2 coordinate for grass */
              z2;                      /**< Z2 coordinate for grass */

      //interPlane* actualPlane;         /**< actual Plane */
};

}

#endif

