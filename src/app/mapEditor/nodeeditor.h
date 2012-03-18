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

#ifndef _dnt_map_editor_node_editor_h
#define _dnt_map_editor_node_editor_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>

#include "../../etc/scenenode.h"
#include "../../map/map.h"
#include "../../engine/character.h"
#include "../../gui/farso.h"

enum
{
   NODE_PARENT_OBJECT,
   NODE_PARENT_CHARACTER,
   NODE_PARENT_DOOR
};

/*! An editor to a scene node */
class nodeEditor
{
   public:
      /*! Constructor */
      nodeEditor(Farso::GuiInterface* g);

      /*! Destructor */
      ~nodeEditor();

      /*! Clear the nodeEditor (usually, after a map open) */
      void clear();

      /*! Set the current map, npc's list and clear */
      void setMap(Map* m, characterList* npcList);

      /*! Flush not applied changes to the map
       * \note: usually called before save the map */
      void flush();

      /*! Select a node to edit (unselecting any previous one)
       * \param scNode -> sceneNode to edit */
      void selectNode(sceneNode* scNode);

      /*! Verify and do action by mouse and state 
       * \param keys -> current keyboard state
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate 
       * \param mouseZ -> mouse Z coordinate 
       * \param mouseXw -> x window mouse coordinate
       * \param mouseYw -> y window mouse coordinate
       * \param mButton -> mouse button state
       * \param proj -> projection matrix
       * \param modl -> modelview matrix
       * \param viewPort -> viewport matrix */
      void verifyAction(Uint8* keys, GLfloat mouseX, GLfloat mouseY, 
            GLfloat mouseZ, Uint8 mButton, int mouseXw, int mouseYw,
            GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! Open the node window */
      void openWindow();

      /*! Treat GUI events
       * \return true if some event occurred here */
      bool eventGot(int eventInfo, Farso::GuiObject* obj);

      /*! Draw temporary elements */
      void drawTemporary();

   protected:

      /*! Delete the current node */
      void deleteCurNode();

      sceneNode* curNode;  /**< current node to edit */
      int nodeParentType; /**< Type of the node(object, character, door, etc)*/
      void* curNodeParent;  /**< Pointer to the node parent */

      Map* curMap;         /**< Current map pointer */
      characterList* npcs; /**< List of NPCs */

      Farso::GuiInterface* gui;   /**< gui used */
      Farso::Window* nodeWindow;  /**< Node info window */
 
      Farso::TabButton* nodeTab;     /**< Tab button for node editor */
      Farso::OneTabButton* rotX[2];  /**< Buttons for X rotation */
      Farso::OneTabButton* rotY[2];  /**< Buttons for Y rotation */
      Farso::OneTabButton* rotZ[2];  /**< Buttons for Z rotation */

      Farso::OneTabButton* moveX[2]; /**< Translate on X */
      Farso::OneTabButton* moveY[2]; /**< Translate on Y */
      Farso::OneTabButton* moveZ[2]; /**< Translate on Z */

      Farso::OneTabButton* clearRot; /**< Clear Rotation button */
      Farso::OneTabButton* deleteNode; /**< Delete Node Button */

      Farso::CxSel* gridMode;        /**< Selector if will grid mode or not */

      Farso::TextBar* deltaText;    /**< Delta Value display */
      float deltaValue;      /**< Delta to move/rotate */
};

#endif

