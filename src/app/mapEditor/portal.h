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

#ifndef _dnt_map_editor_portal_h
#define _dnt_map_editor_portal_h

#include "../../map/map.h"
#include "area.h"
#include "message.h"
#include "../../classes/object.h"
#include "../../engine/util.h"

namespace dntMapEditor
{

class Portal
{
   public:
      /*! Constructor
       * \param acMap -> poiter to opened map */
      Portal(Map* acMap);
      /*! Destructor */
      ~Portal();

      /*! Verify and do action by mouse and state 
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate 
       * \param mouseZ -> mouse Z coordinate 
       * \param mButton -> mouse button state
       * \param tool -> current Tool*/
      void verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, int tool,
                        GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! If have some temporary things to draw, draw it! */
      void drawTemporary();

      /*! Verify if Point is inner the Square */
      bool inner(GLfloat ax, GLfloat az, GLfloat bx1, GLfloat bz1, 
                                        GLfloat bx2, GLfloat bz2);

      void defineDoor(object* newDoor, std::string fileName);

      /*! Get the current door */
      object* getDoor();

      /*! get the current door file name */
      std::string getDoorFileName();

      /*! delete the current door */
      void deleteDoor();


   private:
      Map* actualMap;         /**< Actual Internal Map */
      object* actualDoor;     /**< Actual Door */
      std::string fileDoor;   /**< Actual Door File */
      int doorMode;           /**< Actual Door Mode */
      int doorOrientation;    /**< Actual Door Orientation */
      GLfloat doorX;          /**< Actual Door X position */
      GLfloat doorZ;          /**< Actual Door Z position */
      wall* doorWall;         /**< Actual Door Wall */
      int actualTool;         /**< Actual Tool */
      int state;              /**< Actual Internal State */

      GLfloat mX, mY, mZ;     /**< Mouse Coordinates */

      Uint8 mB;               /**< Mouse Buttons */

      GLfloat initmX, initmZ; /**< Mouse Initial Positions on some States */

      dntMapEditor::AreaList* portalList;   /**< Portal List */

      /*! Internal Add Portal */
      void doAddPortal();
      /*! Internal Tag Portal */
      void doTagPortal(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);
      /*! Add Portal To Map */
      void addPortal(int qx, int qz, std::string where);
};

}

#endif

