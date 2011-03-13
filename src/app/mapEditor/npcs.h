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
  along with DccNitghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_map_editor_npcs_h
#define _dnt_map_editor_npcs_h

#include "../../map/map.h"
#include "../../etc/scene.h"
#include "../../engine/util.h"
#include "../../engine/character.h"
#include "../../classes/feats.h"
#include "message.h"


/*! The Npcs MapEditor Controller Class */
class npcs
{
   public:
      /*! Constructor
       * \param acMap -> pointer to opened map
       * \param NPCsList -> pointer to the list of NPCs 
       * \param lFeats -> pointer to features descriptions list*/
      npcs(Map* acMap, characterList* NPCsList, featsList* lFeats);
      /*! Destructor */
      ~npcs();

      /*! Verify and do action by mouse and state 
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate 
       * \param mouseZ -> mouse Z coordinate 
       * \param mButton -> mouse button state
       * \param tool -> current Tool*/
      void verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, int mouseXw, int mouseYw, int tool,
                        GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! If have some temporary things to draw, draw it! */
      void drawTemporary();

      /*! Define the actual npc */
      void defineActualNpc(string fileName);

      /*! Get the actual fileName of the actual NPC */
      string getNpcFileName();

      /*! Save The NPCs list to a file
       * \param fileName -> name of the file to save */
      bool saveFile(string fileName);
                        
   private:
      characterList* NPCs;        /**< Actual NPCs List */
      characterList* intList;     /**< Internal NPCs List */
      featsList* features;        /**< Actual Features */
      Map* actualMap;             /**< Actual Internal Map */
      character* actualNpc;       /**< Actual Npc */
      string npcFile;             /**< Actual Npc Filename */
      int actualTool;             /**< Actual Tool */
      int state;                  /**< Actual Internal State */

      GLfloat mX, mY, mZ;         /**< Mouse Coordinates */

      Uint8 mB;                   /**< Mouse Buttons */

      void insertNpc(GLfloat xReal, GLfloat zReal,
                     character* npc, int qx, int qz);
};

#endif

