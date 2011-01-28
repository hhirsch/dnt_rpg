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

#ifndef _dnt_part_editor_part_window
#define _dnt_part_editor_part_window

#include <string>
using namespace std;

#include "../../gui/farso.h"
#include "../../particle/particle.h"

#include "partaux.h"
#include "partelementwindow.h"
#include "partoriginwindow.h"

/*! The particle editor window! */
class partWindow
{
   public:
      /*! Constructor 
       * \param interf -> pointer to the current guiInterface */
      partWindow(guiInterface* interf);

      /*! Destructor */
      ~partWindow();
      
      /*! Set the currentparticle to edit
       * \param p -> current particleSystem to edit */
      void set(partAux* p);

      /*! Treat the window events
       * \param object -> guiObject where event occurred
       * \param eventInfo -> info about the happened event
       * \return true when event is related to partWindow */
      bool treat(guiObject* object, int eventInfo);

      /*! Verify if window is open
       * \return true if open */
      bool isOpen();

   protected:
      /*! Open the window with current element */
      void openWindow();

      /*! Close the window */
      void closeWindow();

      /*! Open the window to select texture */
      void openTextureWindow();

      /* Set all texts */
      void setTextValues();
   
      /* Set mode names */
      void setModeNames();

      partElementWindow* elementWindow; /**< The window to edit elements */
      partOriginWindow* originWindow;   /**< The window to edit origin */

      window* textureWindow;            /**< The window to select textures */
      fileSel* fileSelector;            /**< The texture file selector */

      window* curWindow;       /**< Internal window */
      guiInterface* gui;       /**< The guiInterface used */

      partAux* part;           /**< The particle system to edit */

      textBar* maxParticles;
      textBar* maxLifeTime;
      textBar* particleLifeTime;
      textBar* gravity;

      button* previousDrawMode;
      textBox* drawMode;
      button* nextDrawMode;

      button* texture;
      button* restart;

      button* previousRenderMode;
      textBox* renderMode;
      button* nextRenderMode;

      textBar* pointSize;

      cxSel* windAffect;
      cxSel* floorCollision;

      button* element;
      button* origin;

};

#endif

