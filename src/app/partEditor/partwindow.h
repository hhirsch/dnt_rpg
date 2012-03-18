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

#ifndef _dnt_part_editor_part_window
#define _dnt_part_editor_part_window

#include <string>

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
       * \param interf -> pointer to the current Farso::GuiInterface */
      partWindow(Farso::GuiInterface* interf);

      /*! Destructor */
      ~partWindow();
      
      /*! Set the currentparticle to edit
       * \param p -> current particleSystem to edit */
      void set(partAux* p);

      /*! Treat the window events
       * \param object -> Farso::GuiObject where event occurred
       * \param eventInfo -> info about the happened event
       * \return true when event is related to partWindow */
      bool treat(Farso::GuiObject* object, int eventInfo);

      /*! Verify if window is open
       * \return true if open */
      bool isOpen();

      /*! Write current number of particles */
      void writeCurParticles();

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

      Farso::Window* textureWindow;  /**< The window to select textures */
      Farso::FileSel* fileSelector;      /**< The texture file selector */

      Farso::Window* curWindow;       /**< Internal window */
      Farso::GuiInterface* gui;       /**< The Farso::GuiInterface used */

      partAux* part;           /**< The particle system to edit */
      int max;                 /**< Max number of particles got for current
                                    particle system */

      Farso::TextBox* curParticles;   /**< Current number of particles */

      Farso::TextBar* maxParticles;
      Farso::TextBar* maxLifeTime;
      Farso::TextBar* particleLifeTime;
      Farso::TextBar* gravity;

      Farso::Button* previousDrawMode;
      Farso::TextBox* drawMode;
      Farso::Button* nextDrawMode;

      Farso::Button* texture;
      Farso::Button* restart;

      Farso::Button* previousRenderMode;
      Farso::TextBox* renderMode;
      Farso::Button* nextRenderMode;

      Farso::TextBar* pointSize;

      Farso::CxSel* windAffect;
      Farso::CxSel* floorCollision;

      Farso::Button* element;
      Farso::Button* origin;

};

#endif

