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

#ifndef _dnt_map_editor_node_editor_h
#define _dnt_map_editor_node_editor_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>

#include "../../etc/scenenode.h"
#include "../../map/map.h"
#include "../../gui/farso.h"

/*! An editor to a scene node */
class nodeEditor
{
   public:
      /*! Constructor */
      nodeEditor(guiInterface* g);

      /*! Destructor */
      ~nodeEditor();

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

   protected:

      sceneNode* curNode;  /**< current node to edit */

      guiInterface* gui;   /**< gui used */
      window* nodeWindow;  /**< Node info window */
 
      tabButton* nodeTab;     /**< Tab button for node editor */
      oneTabButton* rotX[2];  /**< Buttons for X rotation */
      oneTabButton* rotY[2];  /**< Buttons for Y rotation */
      oneTabButton* rotZ[2];  /**< Buttons for Z rotation */

      oneTabButton* moveX[2]; /**< Translate on X */
      oneTabButton* moveY[2]; /**< Translate on Y */
      oneTabButton* moveZ[2]; /**< Translate on Z */

      oneTabButton* clearRot; /**< Clear Rotation button */
};

#endif

