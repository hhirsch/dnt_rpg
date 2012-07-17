/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_map_editor_sound_editor_h
#define _dnt_map_editor_sound_editor_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>

#include "../../gui/farso.h"
#include "../../sound/sound.h"
#include "../../map/mapsound.h"
#include "../../map/map.h"

#include <string>

namespace dntMapEditor
{

#define SOUND_EDITOR_ACTION_NONE    0
#define SOUND_EDITOR_ACTION_ADDED   1
#define SOUND_EDITOR_ACTION_OTHER   2

/*! An editor to map's sounds */
class SoundEditor
{
   public:
      /*! Constructor */
      SoundEditor(Farso::GuiInterface* g);

      /*! Destructor */
      ~SoundEditor();

      /*! Clear the SoundEditor (usually, after a map open) */
      void clear();

      /*! Set the current map and clear */
      void setMap(Map* m);

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
      int verifyAction(Uint8* keys, GLfloat mouseX, GLfloat mouseY, 
            GLfloat mouseZ, GLfloat xFloor, GLfloat zFloor,
            Uint8 mButton, int tool);

      /*! Treat GUI events
       * \return true if some event occurred here */
      bool eventGot(int eventInfo, Farso::GuiObject* obj);

      /*! Draw temporary elements */
      void drawTemporary();

      /*! Clear no more needed temporary */
      void clearTemporary();

   protected:

      /*! Open the edit window for current sound */
      void openWindow();

      /*! Open file selector window */
      void openFileWindow();

      soundInfo* curSound; /**< Current editing sound */
      sceneNode* curNode;  /**< current node of the sound to edit */
      sceneNode* tmpNode;  /**< Temporary node on sound insert */

      Map* curMap;         /**< Current map pointer */

      Farso::GuiInterface* gui;   /**< gui used */
      Farso::Window* sndWindow;   /**< Sound edit window */
 
      Farso::TabButton* sndTab;      /**< Tab button for node editor */
      Farso::OneTabButton* moveX[2]; /**< Translate on X */
      Farso::OneTabButton* moveY[2]; /**< Translate on Y */
      Farso::OneTabButton* moveZ[2]; /**< Translate on Z */
      Farso::OneTabButton* deleteSnd; /**< Delete Snd Button */
      
      Farso::TextBar* sndFileText;     /**< Text with sound file */
      Farso::Button* sndFileSelButton; /**< Button to open a file selector */

      Farso::CxSel* cxSelSndLoop;       /**< If sound will loop or not */
      Farso::TextBar* loopIntervalText; /**< Interval before loop again text */

      Farso::TextBar* rollOffText;      /**< Current rollOff */
      Farso::TextBar* refDistanceText;  /**< Current refDistance */
      Farso::TextBar* volumeText;       /**< Current volume */

      Farso::Window* fileWindow;        /**< Window for file selector */
      Farso::FileSel* fileSelector;     /**< The file selector */
};

}

#endif

