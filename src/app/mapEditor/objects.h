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

#ifndef _dnt_map_editor_objects_h
#define _dnt_map_editor_objects_h

#include "../../map/map.h"
#include "../../etc/scene.h"
#include "../../engine/util.h"
#include "../../classes/object.h"
#include "message.h"

/*! The objects Map Controller Class */
class objects
{
   public:
      /*! Constructor
       * \param acMap -> poiter to opened map*/
      objects(Map* acMap);
      /*! Destructor */
      ~objects();

      /*! Verify and do action by mouse and state 
       * \param keys -> current keyboard state
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate 
       * \param mouseZ -> mouse Z coordinate 
       * \param mButton -> mouse button state
       * \param tool -> current Tool*/
      void verifyAction(Uint8* keys, GLfloat mouseX, GLfloat mouseY, 
                        GLfloat mouseZ, 
                        Uint8 mButton, int mouseXw, int mouseYw, int tool,
                        GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! If have some temporary things to draw, draw it! */
      void drawTemporary();

      /*! Define the current object to add */
      void defineActualObject(object* obj, string fileName);

      /*! Get object file name
       * \return fileName of object or "" if no object */
      string getObjectFileName();

      /*! delete actual object (if any) */
      void deleteObject();

    private:
      Map* actualMap;             /**< Actual Internal Map */
      object* actualObstacle;     /**< Actual Obstacle */
      string objectFile;          /**< Actual Obstacle Filename */
      int actualTool;             /**< Actual Tool */
      int state;                  /**< Actual Internal State */

      GLfloat mX, mY, mZ;         /**< Mouse Coordinates */

      Uint8 mB;                   /**< Mouse Buttons */

      GLfloat obstacleX,          /**< X position Obstacle */
              obstacleY,          /**< Y position Obstacle */
              obstacleZ;          /**< Z position Obstacle */
      GLfloat angleX,
              angleY,
              angleZ;

      void insertObject(Map* acMap, object* obj, int qx, int qz);

};

#endif

